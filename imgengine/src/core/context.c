#include "imgengine/context.h"

void img_ctx_init(img_ctx_t *ctx, size_t pool_size)
{
    mp_init(&ctx->pool, pool_size);
}

void img_ctx_destroy(img_ctx_t *ctx)
{
    mp_destroy(&ctx->pool);
}