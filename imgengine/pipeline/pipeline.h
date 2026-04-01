/* pipeline/pipeline.h */
#ifndef IMGENGINE_PIPELINE_H
#define IMGENGINE_PIPELINE_H

#include <stdint.h>
#include "core/context.h"
#include "core/image.h"
#include "hot/batch.h"

#define MAX_OPCODES 256

// --- Function Pointer Types ---
typedef void (*img_op_fn)(img_ctx_t *ctx, img_buffer_t *buf, void *params);
typedef void (*img_batch_op_fn)(img_ctx_t *ctx, img_batch_t *batch, void *params);

// --- Global Jump Tables ---
extern img_op_fn g_jump_table[MAX_OPCODES];
extern img_batch_op_fn g_batch_jump_table[MAX_OPCODES];

// --- DAG Node ---
typedef struct
{
    uint32_t op_code;
    void *params;
} img_dag_node_t;

// --- Pipeline Graph ---
typedef struct
{
    img_dag_node_t nodes[16];
    uint32_t node_count;
} img_pipeline_graph_t;

#endif