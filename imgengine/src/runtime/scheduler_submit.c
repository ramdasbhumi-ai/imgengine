// ./src/runtime/scheduler_submit.c
#include "runtime/scheduler.h"
#include "runtime/worker.h"

int img_scheduler_submit(img_scheduler_t *sched, img_task_t *task)
{
    if (!sched || !task)
        return -1;

    uint32_t target = (uintptr_t)task % sched->worker_count;
    img_worker_t *w = &sched->workers[target];

    if (img_queue_push(w->queue, task))
        return 0;

    return img_mpmc_push(&sched->global_queue, task);
}
