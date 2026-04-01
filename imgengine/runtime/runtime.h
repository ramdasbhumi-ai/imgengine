// /* runtime/runtime.h */
// #ifndef IMGENGINE_RUNTIME_INTERNAL_H
// #define IMGENGINE_RUNTIME_INTERNAL_H

// #include <pthread.h>
// #include <stdbool.h>
// #include <stdint.h>
// #include "src/core/context.h"

// // Task structure sent through the queue
// typedef struct
// {
//     void *raw_input;
//     size_t size;
//     uint32_t op_mask;
// } img_task_t;

// // Worker State
// typedef struct
// {
//     pthread_t thread;
//     uint32_t cpu_id;
//     img_ctx_t *ctx;
//     struct img_queue *task_queue;
//     volatile bool active;
// } img_worker_t;

// // API
// void img_affinity_set(uint32_t cpu_id);
// void *img_worker_loop(void *arg);
// bool img_backpressure_check(struct img_queue *q);

// #endif

/* runtime/runtime.h */
#ifndef IMGENGINE_RUNTIME_INTERNAL_H
#define IMGENGINE_RUNTIME_INTERNAL_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdalign.h>
#include "core/context.h"

// 1. Define the Opaque Queue Type
typedef struct img_queue img_queue_t;

// 2. Task structure
typedef struct
{
    void *raw_input;
    size_t size;
    uint32_t op_mask;
} img_task_t;

// 3. Worker State
typedef struct
{
    pthread_t thread;
    uint32_t cpu_id;
    img_ctx_t *ctx;
    img_queue_t *task_queue; // Using the typedef
    volatile bool active;
} img_worker_t;

// API Prototypes
void img_affinity_set(uint32_t cpu_id);
void *img_worker_loop(void *arg);
bool img_backpressure_check(img_queue_t *q);

#endif
