/* runtime/worker.c */
#include "runtime/worker.h"
#include "runtime/affinity.h"
#include "hot/pipeline_exec.h"
#include "security/sandbox.h"

#include <stdlib.h>

typedef struct
{
    void *input;
} img_task_t;

void *img_worker_loop(void *arg)
{
    img_worker_t *self = (img_worker_t *)arg;

    // 1. Pin thread to CPU
    img_affinity_set(self->cpu_id);

    // 2. Enter sandbox (security boundary)
    img_security_enter_sandbox();

    while (self->active)
    {
        img_task_t *task = (img_task_t *)img_queue_pop(self->task_queue);

        if (!task)
        {
            __builtin_ia32_pause();
            continue;
        }

        // 🔥 HOT PATH (Decode → Process → Encode)
        // (Will be wired later)

        free(task);
    }

    return NULL;
}