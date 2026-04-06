// src/pipeline/batch_exec.c

#include "pipeline/batch_exec.h"
#include "pipeline/pipeline_fused.h"
#include "observability/profiler.h"
#include "observability/logger.h"
 #include "core/buffer.h"

void img_batch_execute(
    img_ctx_t *ctx,
    img_batch_t *batch,
    void *pipeline)
{
    (void)pipeline;

    if (__builtin_expect(!batch || batch->count == 0, 0))
        return;

    /*
     * 🔥 Resolve once (NO branch in hot loop)
     */
    img_fused_kernel_fn fn = img_get_fused_kernel();

    if (__builtin_expect(fn == NULL, 0))
        return;

    /*
     * 🔥 BATCH-LEVEL PROFILING (NOT PER IMAGE)
     */
    uint64_t start = img_profiler_now();

    /*
     * 🔥 HOT LOOP (tight, predictable)
     */
    uint32_t count = batch->count;
    img_buffer_t *buffers = batch->buffers;

    for (uint32_t i = 0; i < count; i++)
    {
        fn(ctx, &buffers[i]);
    }

    uint64_t end = img_profiler_now();

    /*
     * 🔥 ZERO-OVERHEAD LOG (OUTSIDE HOT LOOP)
     */
    IMG_LOG_LATENCY(
        end - start,
        count,
        0 /* reserved */
    );
}
