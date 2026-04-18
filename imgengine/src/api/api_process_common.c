// ./src/api/api_process_common.c
#include "api/api_internal.h"

void img_api_make_ctx(img_engine_t *engine, img_ctx_t *ctx)
{
    if (!engine || !ctx)
        return;

    ctx->thread_id = 0;
    ctx->caps = engine->caps;
    ctx->local_pool = engine->global_pool;
    ctx->scratch_arena = NULL;
    ctx->op_params = NULL;
    ctx->fused_params = NULL;
}
