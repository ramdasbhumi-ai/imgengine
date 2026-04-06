// pipeline/pipeline_fused.h

#ifndef IMGENGINE_PIPELINE_FUSED_H
#define IMGENGINE_PIPELINE_FUSED_H

#include <stdint.h>

/* Forward declarations */
typedef struct img_ctx img_ctx_t;
typedef struct img_buffer img_buffer_t;
typedef struct img_batch img_batch_t;
typedef struct img_pipeline_desc img_pipeline_desc_t;

/*
 * 🔥 FUSED OP TYPES (IR)
 */
typedef enum
{
    FUSED_OP_NONE = 0,
    FUSED_OP_GRAYSCALE,
    FUSED_OP_BRIGHTNESS,
} img_fused_op_t;

/*
 * 🔥 LIMITS
 */
#define IMG_MAX_FUSED_OPS 16

/*
 * 🔥 FUSED PIPELINE STRUCT
 */
typedef struct
{
    uint32_t count;

    img_fused_op_t ops[IMG_MAX_FUSED_OPS];
    void *params[IMG_MAX_FUSED_OPS];

    void (*fn)(img_ctx_t *, img_buffer_t *);

} img_pipeline_fused_t;

/*
 * 🔥 API
 */
int img_pipeline_fuse(
    const img_pipeline_desc_t *in,
    img_pipeline_fused_t *out);

void img_pipeline_execute_fused(
    img_ctx_t *ctx,
    img_pipeline_fused_t *pipe,
    img_buffer_t *buf);

void img_pipeline_execute_fused_batch(
    img_ctx_t *ctx,
    img_pipeline_fused_t *pipe,
    img_batch_t *batch);

#endif