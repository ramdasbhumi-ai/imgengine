// /* runtime/worker.c */
// #include "runtime/runtime.h"
// #include "hot/pipeline_exec.h"

// void *img_worker_loop(void *arg)
// {
//     img_worker_t *self = (img_worker_t *)arg;

//     // 1. Pin to core immediately
//     img_affinity_set(self->cpu_id);

//     while (self->active)
//     {
//         // 2. Poll queue (Low-latency spin)
//         img_task_t *task = (img_task_t *)img_queue_pop(self->task_queue);

//         if (!task)
//         {
//             __builtin_ia32_pause(); // CPU hint: Power-efficient spin
//             continue;
//         }

//         // 3. EXECUTE HOT PATH (From Phase 6)
//         // [Decode -> Process -> Encode]

//         free(task); // Task metadata only
//     }
//     return NULL;
// }

/* src/runtime/worker.c */
#include "runtime/runtime.h"
#include "hot/pipeline_exec.h"      // Matches src/hot/pipeline_exec.h
#include "runtime/queue_internal.h" // For img_queue_pop logic
#include <stdlib.h>

void *img_worker_loop(void *arg)
{
    img_worker_t *self = (img_worker_t *)arg;
    img_affinity_set(self->cpu_id);

    while (self->active)
    {
        // Low-latency pop from SPSC queue
        img_task_t *task = (img_task_t *)img_queue_pop(self->task_queue);

        if (!task)
        {
            __builtin_ia32_pause();
            continue;
        }

        /*
         * KERNEL-GRADE: The Hot Path starts here.
         * In a real run, you'd call the decoder, then execute_hot, then encoder.
         */

        free(task);
    }
    return NULL;
}
