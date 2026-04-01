/* pipeline/engine.c */
#include "pipeline/engine.h"
#include "pipeline/jump_table.h"

void img_pipeline_execute(img_ctx_t *ctx,
                          img_pipeline_graph_t *graph,
                          img_buffer_t *buf)
{
    const uint32_t count = graph->node_count;

    for (uint32_t i = 0; i < count; ++i)
    {
        img_op_fn op = g_jump_table[graph->nodes[i].op_code];

        if (__builtin_expect(op != NULL, 1))
        {
            op(ctx, buf, graph->nodes[i].params);
        }
    }
}