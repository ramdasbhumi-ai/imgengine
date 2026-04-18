// ./src/pipeline/layout_resize.c
/* legacy stub; logic moved into one-function translation units

#define _GNU_SOURCE

#include "pipeline/layout_resize.h"

#include <stddef.h>
#include <string.h>

static img_result_t img_layout_resize_validate_alloc(
    const img_buffer_t *src,
    img_buffer_t *dst,
    uint32_t new_w,
    uint32_t new_h,
    img_slab_pool_t *pool,
    uint8_t **mem_out,
    uint32_t *stride_out)
{
    if (!src || !dst || !pool || !src->data || !mem_out || !stride_out)
        return IMG_ERR_SECURITY;
    if (new_w == 0 || new_h == 0)
        return IMG_ERR_SECURITY;
    if (src->width == 0 || src->height == 0 || src->channels < 3)
        return IMG_ERR_FORMAT;

    const uint32_t ch = 3;
    if (new_w > SIZE_MAX / ch)
        return IMG_ERR_NOMEM;

    size_t row_bytes = (size_t)new_w * ch;
    if (new_h > SIZE_MAX / row_bytes)
        return IMG_ERR_NOMEM;

    size_t required = row_bytes * new_h;
    if (row_bytes > UINT32_MAX || required > img_slab_block_size(pool))
        return IMG_ERR_NOMEM;

    uint8_t *mem = img_slab_alloc(pool);
    if (!mem)
        return IMG_ERR_NOMEM;

    *mem_out = mem;
    *stride_out = (uint32_t)row_bytes;
    return IMG_SUCCESS;
}

static void img_layout_resize_resample_bilinear(
    const img_buffer_t *src,
    uint8_t *mem,
    uint32_t new_w,
    uint32_t new_h,
    uint32_t stride)
{
    const uint32_t ch = 3;
    const uint32_t src_w = src->width;
    const uint32_t src_h = src->height;
    const uint32_t x_ratio = (new_w > 1)
                                 ? (uint32_t)(((uint64_t)(src_w - 1) << 16) / (new_w - 1))
                                 : 0;
    const uint32_t y_ratio = (new_h > 1)
                                 ? (uint32_t)(((uint64_t)(src_h - 1) << 16) / (new_h - 1))
                                 : 0;

    for (uint32_t dy = 0; dy < new_h; dy++)
    {
        uint32_t y_fp = dy * y_ratio;
        uint32_t y0 = y_fp >> 16;
        uint32_t y1 = (y0 + 1 < src_h) ? (y0 + 1) : y0;
        uint32_t fy = y_fp & 0xffffu;
        uint32_t wy0 = 65536u - fy;
        const uint8_t *row0 = src->data + (size_t)y0 * src->stride;
        const uint8_t *row1 = src->data + (size_t)y1 * src->stride;
        uint8_t *dst_row = mem + (size_t)dy * stride;

        for (uint32_t dx = 0; dx < new_w; dx++)
        {
            uint32_t x_fp = dx * x_ratio;
            uint32_t x0 = x_fp >> 16;
            uint32_t x1 = (x0 + 1 < src_w) ? (x0 + 1) : x0;
            uint32_t fx = x_fp & 0xffffu;
            uint32_t wx0 = 65536u - fx;

            const uint8_t *p00 = row0 + (size_t)x0 * ch;
            const uint8_t *p10 = row0 + (size_t)x1 * ch;
            const uint8_t *p01 = row1 + (size_t)x0 * ch;
            const uint8_t *p11 = row1 + (size_t)x1 * ch;
            uint8_t *out = dst_row + (size_t)dx * ch;

            for (uint32_t c = 0; c < ch; c++)
            {
                uint64_t top = (uint64_t)p00[c] * wx0 + (uint64_t)p10[c] * fx;
                uint64_t bottom = (uint64_t)p01[c] * wx0 + (uint64_t)p11[c] * fx;
                uint64_t value = top * wy0 + bottom * fy;
                out[c] = (uint8_t)((value + (1ULL << 31)) >> 32);
            }
        }
    }
}

static img_result_t img_layout_prepare_fill_commit(
    const img_buffer_t *tmp,
    img_buffer_t *dst,
    img_slab_pool_t *pool,
    uint32_t cell_w,
    uint32_t cell_h)
{
    if (tmp->width < cell_w || tmp->height < cell_h)
        return IMG_ERR_SECURITY;

    size_t row_bytes = (size_t)cell_w * 3;
    if (row_bytes > UINT32_MAX)
        return IMG_ERR_NOMEM;
    if (cell_h > SIZE_MAX / row_bytes)
        return IMG_ERR_NOMEM;

    size_t required = row_bytes * cell_h;
    if (required > img_slab_block_size(pool))
        return IMG_ERR_NOMEM;

    uint8_t *mem = img_slab_alloc(pool);
    if (!mem)
        return IMG_ERR_NOMEM;

    uint32_t cx = (tmp->width - cell_w) / 2;
    uint32_t cy = (tmp->height - cell_h) / 2;

    dst->data = mem;
    dst->width = cell_w;
    dst->height = cell_h;
    dst->channels = 3;
    dst->stride = (uint32_t)row_bytes;

    for (uint32_t dy = 0; dy < cell_h; dy++)
    {
        const uint8_t *src_row = tmp->data + (cy + dy) * tmp->stride + cx * 3;
        uint8_t *dst_row = mem + dy * dst->stride;
        memcpy(dst_row, src_row, cell_w * 3);
    }

    return IMG_SUCCESS;
}

static img_result_t img_layout_prepare_fit_commit(
    const img_buffer_t *tmp,
    img_buffer_t *dst,
    img_slab_pool_t *pool,
    uint32_t cell_w,
    uint32_t cell_h,
    uint8_t bg_r,
    uint8_t bg_g,
    uint8_t bg_b)
{
    if (tmp->width > cell_w || tmp->height > cell_h)
        return IMG_ERR_SECURITY;

    size_t row_bytes = (size_t)cell_w * 3;
    if (row_bytes > UINT32_MAX)
        return IMG_ERR_NOMEM;
    if (cell_h > SIZE_MAX / row_bytes)
        return IMG_ERR_NOMEM;

    size_t required = row_bytes * cell_h;
    if (required > img_slab_block_size(pool))
        return IMG_ERR_NOMEM;

    uint8_t *mem = img_slab_alloc(pool);
    if (!mem)
        return IMG_ERR_NOMEM;

    for (size_t i = 0; i < required; i += 3)
    {
        mem[i] = bg_r;
        mem[i + 1] = bg_g;
        mem[i + 2] = bg_b;
    }

    dst->data = mem;
    dst->width = cell_w;
    dst->height = cell_h;
    dst->channels = 3;
    dst->stride = (uint32_t)row_bytes;

    uint32_t off_x = (cell_w - tmp->width) / 2;
    uint32_t off_y = (cell_h - tmp->height) / 2;
    for (uint32_t dy = 0; dy < tmp->height; dy++)
    {
        uint8_t *dst_row = mem + (off_y + dy) * dst->stride + off_x * 3;
        const uint8_t *src_row = tmp->data + dy * tmp->stride;
        memcpy(dst_row, src_row, tmp->width * 3);
    }

    return IMG_SUCCESS;
}

img_result_t img_layout_resize_bilinear(
    const img_buffer_t *src,
    img_buffer_t *dst,
    uint32_t new_w,
    uint32_t new_h,
    img_slab_pool_t *pool)
{
    uint8_t *mem = NULL;
    uint32_t stride = 0;
    img_result_t r = img_layout_resize_validate_alloc(
        src, dst, new_w, new_h, pool, &mem, &stride);
    if (r != IMG_SUCCESS)
        return r;

    dst->data = mem;
    dst->width = new_w;
    dst->height = new_h;
    dst->channels = 3;
    dst->stride = stride;

    img_layout_resize_resample_bilinear(src, mem, new_w, new_h, stride);
    return IMG_SUCCESS;
}

img_result_t img_layout_prepare_fill(
    const img_buffer_t *src,
    img_buffer_t *dst,
    uint32_t cell_w,
    uint32_t cell_h,
    img_slab_pool_t *pool)
{
    if (!src || !dst || !pool || !src->data || src->width == 0 || src->height == 0)
        return IMG_ERR_SECURITY;

    float sx = (float)cell_w / (float)src->width;
    float sy = (float)cell_h / (float)src->height;
    float s = (sx > sy) ? sx : sy;

    uint32_t scaled_w = (uint32_t)((float)src->width * s);
    uint32_t scaled_h = (uint32_t)((float)src->height * s);
    if (scaled_w < cell_w)
        scaled_w = cell_w;
    if (scaled_h < cell_h)
        scaled_h = cell_h;

    img_buffer_t tmp = {0};
    img_result_t r = img_layout_resize_bilinear(src, &tmp, scaled_w, scaled_h, pool);
    if (r != IMG_SUCCESS)
        return r;

    r = img_layout_prepare_fill_commit(&tmp, dst, pool, cell_w, cell_h);
    img_slab_free(pool, tmp.data);
    return r;
}

img_result_t img_layout_prepare_fit(
    const img_buffer_t *src,
    img_buffer_t *dst,
    uint32_t cell_w,
    uint32_t cell_h,
    img_slab_pool_t *pool,
    uint8_t bg_r,
    uint8_t bg_g,
    uint8_t bg_b)
{
    if (!src || !dst || !pool || !src->data || src->width == 0 || src->height == 0)
        return IMG_ERR_SECURITY;

    float sx = (float)cell_w / (float)src->width;
    float sy = (float)cell_h / (float)src->height;
    float s = (sx < sy) ? sx : sy;

    uint32_t fit_w = (uint32_t)((float)src->width * s);
    uint32_t fit_h = (uint32_t)((float)src->height * s);
    if (fit_w == 0)
        fit_w = 1;
    if (fit_h == 0)
        fit_h = 1;
    if (fit_w > cell_w)
        fit_w = cell_w;
    if (fit_h > cell_h)
        fit_h = cell_h;

    img_buffer_t tmp = {0};
    img_result_t r = img_layout_resize_bilinear(src, &tmp, fit_w, fit_h, pool);
    if (r != IMG_SUCCESS)
        return r;

    r = img_layout_prepare_fit_commit(&tmp, dst, pool, cell_w, cell_h,
                                      bg_r, bg_g, bg_b);
    img_slab_free(pool, tmp.data);
    return r;
}
*/
