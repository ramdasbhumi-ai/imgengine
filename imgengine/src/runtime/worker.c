/* src/runtime/worker.c */

#include "runtime/worker.h"
#include "runtime/task.h"
#include <unistd.h>

static void *worker_loop(void *arg)
{
    img_worker_t *w = (img_worker_t *)arg;

    while (w->running)
    {
        img_task_t *task =
            (img_task_t *)img_mpmc_pop(w->queue);

        if (!task)
        {
            __builtin_ia32_pause(); // CPU hint
            continue;
        }

        // 🔥 Execute pipeline (your existing engine)
        // pipeline_execute(w->ctx, task);

        task->status = 0;
    }

    return NULL;
}

int img_worker_init(img_worker_t *w, uint32_t id)
{
    img_pin_thread_to_core(id);
    w->id = id;
    w->running = 1;

    pthread_create(&w->thread, NULL, worker_loop, w);
    return 0;
}

void img_worker_stop(img_worker_t *w)
{
    w->running = 0;
}

void img_worker_join(img_worker_t *w)
{
    pthread_join(w->thread, NULL);
}
