// ./src/api/api_job_finish_stage.c
#include "api/api_job_internal.h"

img_result_t img_api_finish_job_stage(
    img_engine_t *engine,
    img_canvas_t *canvas,
    img_arena_t *arena)
{
    if (!engine || !canvas)
        return IMG_ERR_SECURITY;

    if (canvas->buf.data)
        img_canvas_free(canvas, engine->global_pool);

    if (arena)
        img_arena_destroy(arena);

    return IMG_SUCCESS;
}
