#ifndef IMG_CONTEXT_H
#define IMG_CONTEXT_H

#include "memory_pool.h"

typedef struct
{
    mem_pool_t pool;
} img_ctx_t;

void img_ctx_init(img_ctx_t *ctx, size_t pool_size);
void img_ctx_destroy(img_ctx_t *ctx);

#endif