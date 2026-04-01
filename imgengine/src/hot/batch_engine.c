/* src/hot/batch_engine.c */
#include "hot/batch.h"

void img_batch_process_hot(img_ctx_t *ctx, img_batch_t *batch, img_pipeline_t *pipe)
{
    // Prefetch all headers into L1 Cache before starting
    for (uint32_t i = 0; i < batch->count; ++i)
    {
        __builtin_prefetch(batch->buffers[i], 0, 3);
    }

    // Execute the pipeline across the batch
    // This calls Batch-Aware SIMD kernels (AVX-512)
    for (uint32_t j = 0; j < pipe->count; ++j)
    {
        img_batch_op_fn batch_op = g_batch_jump_table[pipe->ops[j].op_code];
        batch_op(ctx, batch, pipe->ops[j].params);
    }
}
