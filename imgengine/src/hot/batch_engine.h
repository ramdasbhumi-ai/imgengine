/* src/hot/batch_engine.h */
#ifndef IMGENGINE_HOT_BATCH_ENGINE_H
#define IMGENGINE_HOT_BATCH_ENGINE_H

#include "hot/batch.h"
#include "pipeline/pipeline.h"

void img_batch_process_hot(img_ctx_t *ctx, img_batch_t *batch, img_pipeline_graph_t *pipe);

#endif
