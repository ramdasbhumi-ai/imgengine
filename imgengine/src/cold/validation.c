// /* src/cold/validation.c */
// #include "validation.h"

// img_result_t img_validate_header(uint32_t w, uint32_t h, uint32_t channels)
// {
//     if (w == 0 || h == 0 || w > MAX_IMG_DIM || h > MAX_IMG_DIM)
//         return IMG_ERR_FORMAT;
//     if (channels != 3 && channels != 4)
//         return IMG_ERR_FORMAT;
//     return IMG_SUCCESS;
// }

/* src/cold/validation.c */
#include "src/cold/validation.h"

img_result_t img_validate_header(uint32_t w, uint32_t h, uint32_t channels)
{
    if (w == 0 || h == 0 || w > MAX_IMG_DIM || h > MAX_IMG_DIM)
        return IMG_ERR_FORMAT;
    if (channels != 3 && channels != 4)
        return IMG_ERR_FORMAT;
    return IMG_SUCCESS;
}

bool img_validate_pipeline_safety(const img_pipeline_graph_t *pipe)
{
    if (!pipe || pipe->node_count == 0 || pipe->node_count > 16)
        return false;

    // L7 Security: Ensure every OpCode in the graph is registered
    for (uint32_t i = 0; i < pipe->node_count; i++)
    {
        if (pipe->nodes[i].op_code == 0 || pipe->nodes[i].op_code >= MAX_OPCODES)
            return false;
    }

    return true;
}
