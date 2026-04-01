/* pipeline/jump_table.c */
#include "pipeline/jump_table.h"

// Global tables (O(1) dispatch)
img_op_fn g_jump_table[MAX_OPCODES] = {0};
img_batch_op_fn g_batch_jump_table[MAX_OPCODES] = {0};

void img_pipeline_register_op(uint32_t op_code,
                              img_op_fn single_fn,
                              img_batch_op_fn batch_fn)
{
    if (__builtin_expect(op_code < MAX_OPCODES, 1))
    {
        g_jump_table[op_code] = single_fn;
        g_batch_jump_table[op_code] = batch_fn;
    }
}