// src/api/api.c

#include "imgengine/api.h"
#include "imgengine/error.h"
#include "imgengine/image.h"
#include "imgengine/io.h"
#include "imgengine/crop.h"
#include "imgengine/resize.h"
#include "imgengine/border.h"
#include "imgengine/layout.h"

#include <stdio.h>

int imgengine_run(img_ctx_t *ctx,
                  const char *input,
                  const char *output,
                  const img_job_t *job)
{

    if (!input || !output || !job)
        return IMG_ERR_INVALID;

    if (job->dpi <= 0 || job->photo_w_cm <= 0 || job->photo_h_cm <= 0)
        return IMG_ERR_INVALID;

    img_t src;
    if (!img_load(input, &src))
        return IMG_ERR_IO;

    int pw = (job->photo_w_cm / 2.54f) * job->dpi;
    int ph = (job->photo_h_cm / 2.54f) * job->dpi;

    img_t cropped;
    if (!img_center_crop(&src, &cropped, &ctx->pool, pw, ph))
        return IMG_ERR_ALLOC;

    img_t resized;
    if (!img_resize(&cropped, &resized, &ctx->pool, pw, ph))
        return IMG_ERR_ALLOC;

    img_t bordered;
    if (!img_add_border(&resized, &bordered, &ctx->pool, job->border_px))
        return IMG_ERR_ALLOC;

    img_t canvas;
    if (!img_create(&canvas, &ctx->pool, 2480, 3508, 3))
        return IMG_ERR_ALLOC;

    // img_t padding;
    // add padding here

    for (int i = 0; i < 2480 * 3508 * 3; i++)
        canvas.data[i] = 255;

    int cols = job->cols;
    int rows = job->rows;
    printf("Manual cols=%d rows=%d auto_fit=%d\n", job->cols, job->rows, job->auto_fit);

    // if (job->auto_fit)
    //     // printf("Auto-fit enabled → ignoring manual cols/rows\n");
    //     printf("Final layout cols=%d rows=%d\n", cols, rows);
    // {
    // layout_autofit(canvas.width, canvas.height,
    //                bordered.width, bordered.height,
    //                job->gap, job->padding,
    //                &cols, &rows);
    // }

    if (job->auto_fit)
    {
        printf("⚠ Auto-fit enabled → overriding cols/rows\n");

        layout_autofit(canvas.width, canvas.height,
                       bordered.width, bordered.height,
                       job->gap, job->padding,
                       &cols, &rows);
    }

    if (!layout_grid(&canvas, &bordered, cols, rows, job->gap, job->padding))
        return IMG_ERR_INVALID;

    if (!img_save_png(output, &canvas))
        return IMG_ERR_IO;

    return IMG_OK;
}