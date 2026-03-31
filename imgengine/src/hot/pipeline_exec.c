/* src/hot/pipeline_exec.c */
#include "pipeline/engine.h"
#include "pipeline/jump_table.h"

// The "Hot" Execution Loop
void img_pipeline_execute_hot(img_ctx_t *ctx, img_pipeline_t *pipe, img_buffer_t *buf)
{
    const uint32_t count = pipe->count;
    const img_instruction_t *restrict ops = pipe->ops;

    for (uint32_t i = 0; i < count; ++i)
    {
        // O(1) Dispatch via Jump Table
        // __builtin_assume helps the compiler eliminate bounds checks
        img_op_fn op_fn = g_jump_table[ops[i].op_code];

        // EXECUTE: Direct jump to SIMD/Scalar kernel
        op_fn(ctx, buf, ops[i].params);
    }
}
