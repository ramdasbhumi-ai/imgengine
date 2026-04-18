// ./src/api/api_process_fast_pipeline.c
#include "api/api_internal.h"
#include "cold/validation.h"
#include "hot/pipeline_exec.h"
#include "pipeline/pipeline_compiled.h"

img_result_t img_api_process_fast_run(
    img_engine_t *engine,
    img_pipeline_desc_t *pipe,
    img_buffer_t *out_buf)
{
    if (!engine || !pipe || !out_buf)
        return IMG_ERR_SECURITY;

    img_pipeline_compiled_t compiled;
    if (!img_validate_pipeline_safety(pipe))
        return IMG_ERR_SECURITY;

    img_result_t r = (img_pipeline_compile(pipe, &compiled) == 0)
                         ? IMG_SUCCESS
                         : IMG_ERR_FORMAT;
    if (r != IMG_SUCCESS)
        return r;

    img_ctx_t ctx = {0};
    img_api_make_ctx(engine, &ctx);

    img_pipeline_execute_hot(
        &ctx,
        (img_pipeline_runtime_t *)&compiled,
        out_buf);

    return IMG_SUCCESS;
}
