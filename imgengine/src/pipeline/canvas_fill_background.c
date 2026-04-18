// ./src/pipeline/canvas_fill_background.c
#include "pipeline/canvas_internal.h"

void img_canvas_fill_background(
    img_canvas_t *canvas,
    const img_job_t *job)
{
    size_t required = (size_t)canvas->buf.width * canvas->buf.height * 3;
    uint8_t *mem = canvas->buf.data;

    for (size_t i = 0; i < required; i += 3)
    {
        mem[i] = job->bg_r;
        mem[i + 1] = job->bg_g;
        mem[i + 2] = job->bg_b;
    }
}
