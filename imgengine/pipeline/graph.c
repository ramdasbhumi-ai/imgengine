/* pipeline/graph.c */
#include "pipeline/pipeline.h"
#include "api/v1/img_error.h"

img_result_t img_graph_compile(img_pipeline_graph_t *graph, const uint32_t *op_codes, uint32_t count)
{
    if (count > 16)
        return IMG_ERR_FORMAT;

    for (uint32_t i = 0; i < count; i++)
    {
        // Validate that the op exists in the jump table before starting
        if (g_jump_table[op_codes[i]] == NULL)
            return IMG_ERR_HW_UNSUP;
        graph->nodes[i].op_code = op_codes[i];
    }
    graph->node_count = count;
    return IMG_SUCCESS;
}
