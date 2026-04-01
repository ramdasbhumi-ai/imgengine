/* pipeline/scheduler.c */
#include "pipeline/pipeline.h"
#include "pipeline/scheduler.h"
#include "hot/batch.h"

void img_scheduler_dispatch_batch(img_ctx_t *ctx, img_batch_t *batch, img_pipeline_graph_t *graph)
{
    for (uint32_t i = 0; i < graph->node_count; i++)
    {
        // Jump Table Dispatch (O(1))
        img_batch_op_fn b_op = g_batch_jump_table[graph->nodes[i].op_code];
        if (b_op)
        {
            b_op(ctx, batch, graph->nodes[i].params);
        }
    }
}
