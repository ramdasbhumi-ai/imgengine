#include "cold/debug.h"
#include <stdio.h>

void img_debug_dump_ctx(const img_ctx_t *ctx)
{
    printf("[DEBUG] Thread: %u Pool: %p Caps: %u\n",
           ctx->thread_id,
           (void *)ctx->pool,
           ctx->cpu_caps);
}