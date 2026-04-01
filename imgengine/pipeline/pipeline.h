// // /* pipeline/pipeline.h */
// // #ifndef IMGENGINE_PIPELINE_INTERNAL_H
// // #define IMGENGINE_PIPELINE_INTERNAL_H

// // #include "src/core/context.h"
// // #include "core/image.h"

// // #define MAX_OPCODES 256

// // // Function pointer types for Single and Batch execution
// // typedef void (*img_op_fn)(img_ctx_t *ctx, img_buffer_t *buf, void *params);
// // typedef void (*img_batch_op_fn)(img_ctx_t *ctx, img_ctx_t *batch, void *params);

// // // The Jump Table (Global for O(1) Access)
// // extern img_op_fn g_jump_table[MAX_OPCODES];
// // extern img_batch_op_fn g_batch_jump_table[MAX_OPCODES];

// // // DAG Node Representation
// // typedef struct
// // {
// //     uint32_t op_code;
// //     void *params;
// // } img_dag_node_t;

// // typedef struct
// // {
// //     img_dag_node_t nodes[16]; // Fixed size to avoid heap allocation
// //     uint32_t node_count;
// // } img_pipeline_graph_t;

// // #endif

// /* pipeline/pipeline.h */
// #ifndef IMGENGINE_PIPELINE_INTERNAL_H
// #define IMGENGINE_PIPELINE_INTERNAL_H

// #include "core/context.h"
// #include "core/image.h"

// #define MAX_OPCODES 256

// // --- OpCode Definitions (The Engine's Commands) ---
// #define OP_RESIZE 0x01
// #define OP_CROP 0x02
// #define OP_GRAY 0x03

// // Function pointer types
// typedef void (*img_op_fn)(img_ctx_t *ctx, img_buffer_t *buf, void *params);
// typedef void (*img_batch_op_fn)(img_ctx_t *ctx, img_buffer_t *batch, void *params); // Fixed: should be batch pointer

// // The Jump Table
// extern img_op_fn g_jump_table[MAX_OPCODES];
// extern img_batch_op_fn g_batch_jump_table[MAX_OPCODES];

// // Registration function declaration
// void img_pipeline_register_op(uint32_t op_code, img_op_fn single_fn, img_batch_op_fn batch_fn);

// typedef struct
// {
//     uint32_t op_code;
//     void *params;
// } img_dag_node_t;

// typedef struct
// {
//     img_dag_node_t nodes[16];
//     uint32_t node_count;
// } img_pipeline_graph_t;

// #endif

/* pipeline/pipeline.h */
#ifndef IMGENGINE_PIPELINE_INTERNAL_H
#define IMGENGINE_PIPELINE_INTERNAL_H

#include "core/context.h"
#include "core/image.h"
#include "hot/batch.h" // Ensure this is included for img_batch_t

#define MAX_OPCODES 256
#define OP_RESIZE 0x01

// Function pointer types
typedef void (*img_op_fn)(img_ctx_t *ctx, img_buffer_t *buf, void *params);

/* FIX: Changed argument 2 from img_ctx_t* (typo) or img_buffer_t* to img_batch_t* */
typedef void (*img_batch_op_fn)(img_ctx_t *ctx, img_batch_t *batch, void *params);

extern img_op_fn g_jump_table[MAX_OPCODES];
extern img_batch_op_fn g_batch_jump_table[MAX_OPCODES];

void img_pipeline_register_op(uint32_t op_code, img_op_fn single_fn, img_batch_op_fn batch_fn);

typedef struct
{
    uint32_t op_code;
    void *params;
} img_dag_node_t;

typedef struct
{
    img_dag_node_t nodes[16];
    uint32_t node_count;
} img_pipeline_graph_t;

#endif
