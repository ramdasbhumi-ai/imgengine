

/* src/cold/debug.c */
#include "debug.h"
#include <stdio.h>

void img_debug_dump_slab_usage(const img_ctx_t* ctx) {
    // Cold path: printf is okay here, never in the hot path.
    printf("[DEBUG] Worker %u | Slab Pool: %p | Caps: 0x%x\n", 
           ctx->worker_id, (void*)ctx->pool, ctx->cpu_caps);
}
