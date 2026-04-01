/* pipeline/jump_table.c */
#include "pipeline/pipeline.h"
#include "pipeline/jump_table.h"

// Global O(1) Tables - These are the actual memory locations
img_op_fn g_jump_table[MAX_OPCODES] = {0};
img_batch_op_fn g_batch_jump_table[MAX_OPCODES] = {0};

/**
 * @brief Registers a single and/or batch operation into the global Jump Table.
 * Optimized with compiler hints for the cold-path boot sequence.
 */
void img_pipeline_register_op(uint32_t op_code, img_op_fn single_fn, img_batch_op_fn batch_fn)
{
    // L7 Optimization: Tell the CPU the opcode is expected to be valid (< 256)
    if (__builtin_expect(op_code < MAX_OPCODES, 1))
    {
        g_jump_table[op_code] = single_fn;
        g_batch_jump_table[op_code] = batch_fn;
    }
}
