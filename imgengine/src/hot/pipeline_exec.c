// /* src/hot/pipeline_exec.c (Updated) */
// #include "pipeline/engine.h"
// #include "pipeline/jump_table.h"
// #include "observability/tracing.h"

// // The "Hot" Execution Loop
// void img_pipeline_execute_hot(img_ctx_t *ctx, img_pipeline_t *pipe, img_buffer_t *buf)
// {
//     const uint32_t count = pipe->count;
//     const img_instruction_t *restrict ops = pipe->ops;

//     for (uint32_t i = 0; i < count; ++i)
//     {
//         // Start Trace
//         img_span_t span = img_trace_span_start("JUMP_TABLE_EXEC");
//         // O(1) Dispatch via Jump Table
//         // __builtin_assume helps the compiler eliminate bounds checks
//         img_op_fn op_fn = g_jump_table[ops[i].op_code];

//         // EXECUTE: Direct jump to SIMD/Scalar kernel
//         op_fn(ctx, buf, ops[i].params);

//         // End Trace (Async hand-off)
//         img_trace_span_end(span);
//     }
// }

// /* src/hot/pipeline_exec.c (Updated) */
// #include "hot/pipeline_exec.h"
// #include "observability/profiler.h"
// #include "observability/metrics.h"

// void img_pipeline_execute_hot(img_ctx_t *ctx, img_pipeline_graph_t *graph, img_buffer_t *buf) {
//     // START SENSOR
//     uint64_t start_cycles = img_profiler_now();

//     for (uint32_t i = 0; i < graph->node_count; ++i) {
//         img_op_fn op = g_jump_table[graph->nodes[i].op_code];
//         op(ctx, buf, graph->nodes[i].params);
//     }

//     // END SENSOR
//     uint64_t elapsed = img_profiler_now() - start_cycles;
//     img_metrics_observe_latency(elapsed); // Atomic update
//     img_metrics_inc(&g_metrics.total_requests);
// }

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
