// ./src/runtime/scheduler_destroy.c
#include "runtime/scheduler.h"
#include "runtime/worker.h"

#include <stdlib.h>

void img_scheduler_destroy(img_scheduler_t *sched)
{
    if (!sched)
        return;

    for (uint32_t i = 0; i < sched->worker_count; i++)
    {
        img_worker_stop(&sched->workers[i]);
        img_worker_join(&sched->workers[i]);
    }

    img_mpmc_destroy(&sched->global_queue);
    free(sched->workers);
}
