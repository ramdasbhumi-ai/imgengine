/* api/v1/img_pipeline.h */
#ifndef IMGENGINE_API_PIPELINE_H
#define IMGENGINE_API_PIPELINE_H

#include "img_core.h"

// Opaque handle to a pre-compiled execution graph
typedef struct img_pipe_s *img_pipe_t;

/**
 * @brief Compile a sequence of operations into a DAG.
 * Validates hardware support before returning the handle.
 */
img_pipe_t img_api_pipeline_create(img_engine_t engine);

void img_api_pipeline_add_resize(img_pipe_t pipe, uint32_t w, uint32_t h);
void img_api_pipeline_add_grayscale(img_pipe_t pipe);

#endif
