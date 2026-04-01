/* pipeline/scheduler.c */
#include "pipeline/scheduler.h"
#include "pipeline/jump_table.h"

void img_scheduler_dispatch_batch(img_ctx_t *ctx,
                                  img_batch_t *batch,
                                  img_pipeline_graph_t *graph)
{
    for (uint32_t i = 0; i < graph->node_count; i++)
    {
        img_batch_op_fn op = g_batch_jump_table[graph->nodes[i].op_code];

        if (op)
        {
            op(ctx, batch, graph->nodes[i].params);
        }
    }
}