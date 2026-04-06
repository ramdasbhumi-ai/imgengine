/* src/runtime/worker.c */

#include "runtime/worker.h"
#include "runtime/task.h"
#include <unistd.h>
#include "runtime/scheduler.h"
#include "hot/pipeline_exec.h"

static void *worker_loop(void *arg)
{
    img_worker_t *w = (img_worker_t *)arg;

    while (w->running)
    {
        img_task_t *task =
            (img_task_t *)img_queue_pop(w->queue);

        // 🔥 STEAL if empty
        if (!task)
        {
            task = img_scheduler_steal(
                w->scheduler,
                w->id);
        }

        if (!task)
        {
            __builtin_ia32_pause();
            continue;
        }

        // 🔥 EXECUTE (HOT PATH)
        img_pipeline_execute_hot(
            w->ctx,
            task->pipeline,
            task->buffer);

        task->status = 0;
    }

    return NULL;
}

int img_worker_init(img_worker_t *w, uint32_t id)
{
    w->id = id;
    w->running = 1;

    w->queue = img_queue_create(10); // 1024

    img_pin_thread_to_core(id);

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
