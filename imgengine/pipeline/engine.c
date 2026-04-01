/* pipeline/engine.c */
#include "pipeline/pipeline.h"

void img_pipeline_execute(img_ctx_t *ctx, img_pipeline_graph_t *graph, img_buffer_t *buf)
{
    // KERNEL-GRADE: Use restrict to help the compiler optimize the loop
    const uint32_t count = graph->node_count;
    const img_dag_node_t *restrict nodes = graph->nodes;

    for (uint32_t i = 0; i < count; ++i)
    {
        // JUMP: O(1) Dispatch
        img_op_fn op = g_jump_table[nodes[i].op_code];

        if (__builtin_expect(op != NULL, 1))
        {
            op(ctx, buf, nodes[i].params);
        }
    }
}
