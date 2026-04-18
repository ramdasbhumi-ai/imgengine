// ./src/pipeline/layout_resize_resample_bilinear.c
#define _GNU_SOURCE

#include "pipeline/layout_resize_internal.h"

void img_layout_resize_resample_bilinear(
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
