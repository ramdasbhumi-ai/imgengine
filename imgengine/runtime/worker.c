/* runtime/worker.c */
#include "runtime/runtime.h"
#include "hot/pipeline_exec.h"

void *img_worker_loop(void *arg)
{
    img_worker_t *self = (img_worker_t *)arg;

    // 1. Pin to core immediately
    img_affinity_set(self->cpu_id);

    while (self->active)
    {
        // 2. Poll queue (Low-latency spin)
        img_task_t *task = (img_task_t *)img_queue_pop(self->task_queue);

        if (!task)
        {
            __builtin_ia32_pause(); // CPU hint: Power-efficient spin
            continue;
        }

        // 3. EXECUTE HOT PATH (From Phase 6)
        // [Decode -> Process -> Encode]

        free(task); // Task metadata only
    }
    return NULL;
}
