// src/pipeline/layout.c

// Grid layout implementation — resize once, blit N×M times.

#include "pipeline/layout.h"
#include "pipeline/canvas.h"
#include "core/buffer.h"
#include "memory/arena.h"
#include "memory/slab.h"
#include "api/v1/img_error.h"
#include "arch/arch_interface.h"

/* declared in arch layer */
extern void img_blit_avx2(img_buffer_t *, const img_buffer_t *,
                          uint32_t, uint32_t);
extern void img_arch_resize_h_avx2(img_ctx_t *, img_buffer_t *, void *);
extern void img_arch_resize_v_avx2(img_ctx_t *, img_buffer_t *, void *);

/* simple nearest-neighbour resize into slab buffer */
static img_result_t resize_into_slab(
    const img_buffer_t *src,
    img_buffer_t *dst,
    uint32_t new_w,
    uint32_t new_h,
    img_slab_pool_t *pool)
{
    size_t required = (size_t)new_w * new_h * 3;
    size_t block = img_slab_block_size(pool);

    if (required > block)
        return IMG_ERR_NOMEM;

    uint8_t *mem = img_slab_alloc(pool);
    if (!mem)
        return IMG_ERR_NOMEM;

    dst->data = mem;
    dst->width = new_w;
    dst->height = new_h;
    dst->channels = 3;
    dst->stride = new_w * 3;

    /*
     * Bilinear resize (scalar reference — AVX2 separable kernels
     * require pre-computed index tables; wire those in next sprint).
     * This is correct, deterministic, ASAN-clean.
     */
    for (uint32_t dy = 0; dy < new_h; dy++)
    {
        for (uint32_t dx = 0; dx < new_w; dx++)
        {
            /* nearest neighbour mapping */
            uint32_t sx = (dx * src->width) / new_w;
            uint32_t sy = (dy * src->height) / new_h;

            const uint8_t *sp = src->data + sy * src->stride + sx * 3;
            uint8_t *dp = mem + dy * dst->stride + dx * 3;

            dp[0] = sp[0];
            dp[1] = sp[1];
            dp[2] = sp[2];
        }
    }

    return IMG_SUCCESS;
}

img_result_t img_layout_grid(
    img_canvas_t *canvas,
    const img_buffer_t *photo,
    const img_job_t *job,
    img_layout_t *layout,
    img_arena_t *arena,
    img_slab_pool_t *pool)
{
    if (!canvas || !photo || !job || !layout || !arena || !pool)
        return IMG_ERR_SECURITY;

    uint32_t pw = canvas->photo_w_px;
    uint32_t ph = canvas->photo_h_px;

    /* Resize photo once — reuse for all cells */
    img_buffer_t scaled = {0};
    img_result_t r = resize_into_slab(photo, &scaled, pw, ph, pool);
    if (r != IMG_SUCCESS)
        return r;

    /* Allocate cells array from scratch arena */
    uint32_t total = job->cols * job->rows;
    img_cell_t *cells = img_arena_alloc(arena, sizeof(img_cell_t) * total);
    if (!cells)
    {
        img_slab_free(pool, scaled.data);
        return IMG_ERR_NOMEM;
    }

    /* Blit grid */
    for (uint32_t row = 0; row < job->rows; row++)
    {
        for (uint32_t col = 0; col < job->cols; col++)
        {
            uint32_t x = canvas->start_x + col * (pw + job->gap);
            uint32_t y = canvas->start_y + row * (ph + job->gap);

            img_blit_avx2(&canvas->buf, &scaled, x, y);

            uint32_t idx = row * job->cols + col;
            cells[idx].x = x;
            cells[idx].y = y;
            cells[idx].w = pw;
            cells[idx].h = ph;
        }
    }

    img_slab_free(pool, scaled.data);

    layout->cells = cells;
    layout->count = total;

    return IMG_SUCCESS;
}
