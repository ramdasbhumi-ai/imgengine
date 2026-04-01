#include "core/context.h"
#include "memory/slab.h"
#include <stdlib.h>

img_ctx_t *img_ctx_create(uint32_t thread_id)
{
    img_ctx_t *ctx = malloc(sizeof(img_ctx_t));
    if (!ctx)
        return NULL;

    ctx->thread_id = thread_id;
    ctx->pool = img_slab_create(64 * 1024 * 1024); // 64MB
    ctx->cpu_caps = img_detect_cpu();

    return ctx;
}

void img_ctx_destroy(img_ctx_t *ctx)
{
    if (!ctx)
        return;

    img_slab_destroy(ctx->pool);
    free(ctx);
}