/* pipeline/jump_table.h */
#ifndef IMGENGINE_PIPELINE_JUMP_TABLE_H
#define IMGENGINE_PIPELINE_JUMP_TABLE_H

#include "pipeline/pipeline.h"

/**
 * Register operation into jump table
 */
void img_pipeline_register_op(uint32_t op_code,
                              img_op_fn single_fn,
                              img_batch_op_fn batch_fn);

#endif