// src/pipeline/canvas.c
// Canvas allocation and geometry computation.

#include "pipeline/canvas.h"
#include "api/v1/img_error.h"
#include <string.h>

img_result_t img_canvas_init(
    img_canvas_t *canvas,
    img_slab_pool_t *pool,
    const img_job_t *job)
{
    if (!canvas || !pool || !job)
        return IMG_ERR_SECURITY;

    /* A4 page size in pixels */
    uint32_t pw = img_cm_to_px(IMG_A4_W_CM, job->dpi);
    uint32_t ph = img_cm_to_px(IMG_A4_H_CM, job->dpi);

    canvas->page_w_px = pw;
    canvas->page_h_px = ph;

    /* Photo cell size in pixels */
    uint32_t photo_w = img_cm_to_px(job->photo_w_cm, job->dpi);
    uint32_t photo_h = img_cm_to_px(job->photo_h_cm, job->dpi);

    canvas->photo_w_px = photo_w;
    canvas->photo_h_px = photo_h;

    /* Grid geometry */
    uint32_t grid_w = job->cols * photo_w + (job->cols - 1) * job->gap;
    uint32_t grid_h = job->rows * photo_h + (job->rows - 1) * job->gap;

    canvas->start_x = (pw > grid_w) ? (pw - grid_w) / 2 : job->padding;
    canvas->start_y = job->padding;

    /* Allocate from slab */
    size_t required = (size_t)pw * ph * 3; /* RGB */
    size_t block = img_slab_block_size(pool);

    if (required > block)
        return IMG_ERR_NOMEM;

    uint8_t *mem = img_slab_alloc(pool);
    if (!mem)
        return IMG_ERR_NOMEM;

    /* Fill background */
    for (size_t i = 0; i < required; i += 3)
    {
        mem[i] = job->bg_r;
        mem[i + 1] = job->bg_g;
        mem[i + 2] = job->bg_b;
    }

    canvas->buf.data = mem;
    canvas->buf.width = pw;
    canvas->buf.height = ph;
    canvas->buf.channels = 3;
    canvas->buf.stride = pw * 3;

    return IMG_SUCCESS;
}

void img_canvas_free(img_canvas_t *canvas, img_slab_pool_t *pool)
{
    if (canvas && canvas->buf.data && pool)
    {
        img_slab_free(pool, canvas->buf.data);
        canvas->buf.data = NULL;
    }
}
