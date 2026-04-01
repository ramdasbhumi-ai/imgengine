
/* src/hot/pipeline_exec.c */
#include "hot/pipeline_exec.h"
#include "pipeline/jump_table.h"
#include "observability/profiler.h"
#include "observability/metrics.h"
#include "observability/tracing.h"

void img_pipeline_execute_hot(img_ctx_t *ctx, img_pipeline_graph_t *graph, img_buffer_t *buf)
{
    // START SENSOR (Hardware Cycles)
    uint64_t start_cycles = img_profiler_now();

    // START OTel SPAN
    img_span_t span = img_trace_span_start("PIPELINE_EXEC");

    const uint32_t count = graph->node_count;
    for (uint32_t i = 0; i < count; ++i)
    {
        // O(1) JUMP
        img_op_fn op = g_jump_table[graph->nodes[i].op_code];

        // EXECUTE SIMD KERNEL
        if (__builtin_expect(op != NULL, 1))
        {
            op(ctx, buf, graph->nodes[i].params);
        }
    }

    // END OTel SPAN (Async hand-off)
    img_trace_span_end(span);

    // END SENSOR & ATOMIC UPDATE
    uint64_t elapsed = img_profiler_now() - start_cycles;
    img_metrics_observe_latency(elapsed);
    img_metrics_inc(&g_metrics.total_requests);
}
