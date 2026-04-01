/* pipeline/engine.h */
#ifndef IMGENGINE_PIPELINE_ENGINE_H
#define IMGENGINE_PIPELINE_ENGINE_H

#include "pipeline/pipeline.h"

/**
 * Execute pipeline graph (single image)
 */
void img_pipeline_execute(img_ctx_t *ctx,
                          img_pipeline_graph_t *graph,
                          img_buffer_t *buf);

#endif