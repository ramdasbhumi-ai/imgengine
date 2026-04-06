/* pipeline/scheduler.h */

#ifndef IMGENGINE_RUNTIME_SCHEDULER_H
#define IMGENGINE_RUNTIME_SCHEDULER_H

#include <stdint.h>
#include "runtime/task.h"
#include "runtime/queue_spsc.h"
#include "runtime/queue_mpmc.h"

typedef struct img_worker_s img_worker_t;

typedef struct
{
    uint32_t worker_count;

    img_worker_t *workers;

    // 🔥 GLOBAL FALLBACK
    img_mpmc_queue_t global_queue;

} img_scheduler_t;

// lifecycle
int img_scheduler_init(img_scheduler_t *sched, uint32_t workers);

void img_scheduler_destroy(img_scheduler_t *sched);

// submit
int img_scheduler_submit(img_scheduler_t *sched, img_task_t *task);

// steal
img_task_t *img_scheduler_steal(img_scheduler_t *sched, uint32_t self_id);

#endif

// #ifndef IMGENGINE_PIPELINE_SCHEDULER_H
// #define IMGENGINE_PIPELINE_SCHEDULER_H

// // #include "pipeline/pipeline.h"
// // #include "hot/batch.h"
// /*
//  * 🔥 Forward declarations (L8 rule)
//  * Avoid pulling full headers into interface layer
//  */
// typedef struct img_ctx img_ctx_t;
// typedef struct img_batch img_batch_t;

// /**
//  * Dispatch batch pipeline execution
//  */
// void img_scheduler_dispatch_batch(img_ctx_t *ctx,
//                                   img_batch_t *batch);
// //   img_pipeline_graph_t *graph);

// #endif