/* pipeline/pipeline.h */
#ifndef IMGENGINE_PIPELINE_INTERNAL_H
#define IMGENGINE_PIPELINE_INTERNAL_H

#include "core/context.h"
#include "core/image.h"

#define MAX_OPCODES 256

// Function pointer types for Single and Batch execution
typedef void (*img_op_fn)(img_ctx_t *ctx, img_buffer_t *buf, void *params);
typedef void (*img_batch_op_fn)(img_ctx_t *ctx, img_batch_t *batch, void *params);

// The Jump Table (Global for O(1) Access)
extern img_op_fn g_jump_table[MAX_OPCODES];
extern img_batch_op_fn g_batch_jump_table[MAX_OPCODES];

// DAG Node Representation
typedef struct
{
    uint32_t op_code;
    void *params;
} img_dag_node_t;

typedef struct
{
    img_dag_node_t nodes[16]; // Fixed size to avoid heap allocation
    uint32_t node_count;
} img_pipeline_graph_t;

#endif
