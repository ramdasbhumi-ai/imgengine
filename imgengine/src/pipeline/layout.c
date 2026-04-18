// ./src/pipeline/layout.c
/* legacy stub; logic moved into one-function translation units

// src/pipeline/layout.c

// ================================================================
// FILE 1: src/pipeline/layout.c
// Wire job->mode: IMG_FILL = center-crop to fill cell exactly
//                 IMG_FIT  = scale to fit, letterbox with bg color
// ================================================================

#define _GNU_SOURCE

#include "pipeline/layout.h"
#include "pipeline/layout_resize.h"

#include <stddef.h>
#include <limits.h>
#include <string.h>

extern void img_blit_avx2(img_buffer_t *, const img_buffer_t *,
                          uint32_t, uint32_t);

static img_result_t img_layout_grid_prepare_scaled(
    const img_buffer_t *photo,
    const img_job_t *job,
    uint32_t pw,
    uint32_t ph,
    img_slab_pool_t *pool,
    img_buffer_t *scaled)
{
    if (job->mode == IMG_FIT)
    {
        return img_layout_prepare_fit(photo, scaled, pw, ph, pool,
                                      job->bg_r, job->bg_g, job->bg_b);
    }

    return img_layout_prepare_fill(photo, scaled, pw, ph, pool);
}

static img_result_t img_layout_grid_alloc_cells(
    const img_job_t *job,
    img_arena_t *arena,
    img_cell_t **cells_out)
{
    size_t cols = (size_t)job->cols;
    size_t rows = (size_t)job->rows;
    if (rows > SIZE_MAX / cols)
        return IMG_ERR_NOMEM;

    size_t total = cols * rows;
    if (total == 0 || total > UINT32_MAX)
        return IMG_ERR_SECURITY;
    if (total > SIZE_MAX / sizeof(img_cell_t))
        return IMG_ERR_NOMEM;

    size_t cell_bytes = total * sizeof(img_cell_t);
    img_cell_t *cells = img_arena_alloc(arena, cell_bytes);
    if (!cells)
        return IMG_ERR_NOMEM;

    memset(cells, 0, cell_bytes);
    *cells_out = cells;
    return IMG_SUCCESS;
}

static uint32_t img_layout_grid_place_cells(
    img_canvas_t *canvas,
    const img_buffer_t *scaled,
    const img_job_t *job,
    img_cell_t *cells)
{
    uint32_t pw = canvas->photo_w_px;
    uint32_t ph = canvas->photo_h_px;
    uint32_t step_x = pw + job->gap;
    uint32_t step_y = ph + job->gap;
    uint32_t placed = 0;

    for (uint32_t row = 0; row < job->rows; row++)
    {
        for (uint32_t col = 0; col < job->cols; col++)
        {
            size_t x = (size_t)canvas->start_x + (size_t)col * step_x;
            size_t y = (size_t)canvas->start_y + (size_t)row * step_y;

            if (x > UINT32_MAX || y > UINT32_MAX)
                continue;
            if (x > canvas->buf.width || pw > canvas->buf.width - (uint32_t)x)
                continue;
            if (y > canvas->buf.height || ph > canvas->buf.height - (uint32_t)y)
                continue;

            img_blit_avx2(&canvas->buf, scaled, (uint32_t)x, (uint32_t)y);

            size_t idx = (size_t)row * (size_t)job->cols + (size_t)col;
            cells[idx].x = x;
            cells[idx].y = y;
            cells[idx].w = pw;
            cells[idx].h = ph;
            placed++;
        }
    }

    return placed;
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

    layout->cells = NULL;
    layout->count = 0;

    if (job->cols == 0 || job->rows == 0)
        return IMG_ERR_SECURITY;

    uint32_t pw = canvas->photo_w_px;
    uint32_t ph = canvas->photo_h_px;

    img_buffer_t scaled = {0};
    img_result_t r = img_layout_grid_prepare_scaled(
        photo, job, pw, ph, pool, &scaled);

    if (r != IMG_SUCCESS)
        return r;

    img_cell_t *cells = NULL;
    r = img_layout_grid_alloc_cells(job, arena, &cells);
    if (r != IMG_SUCCESS)
    {
        img_slab_free(pool, scaled.data);
        return r;
    }

    if (job->gap > UINT32_MAX - pw || job->gap > UINT32_MAX - ph)
    {
        img_slab_free(pool, scaled.data);
        return IMG_ERR_SECURITY;
    }

    uint32_t placed = img_layout_grid_place_cells(canvas, &scaled, job, cells);

    img_slab_free(pool, scaled.data);
    layout->cells = cells;
    layout->count = placed;
    return IMG_SUCCESS;
}
*/
