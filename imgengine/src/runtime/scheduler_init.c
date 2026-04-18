// ./src/runtime/scheduler_init.c
#include "runtime/scheduler.h"
#include "runtime/worker.h"

#include <stdlib.h>

int img_scheduler_init(img_scheduler_t *sched, uint32_t workers)
{
    if (!sched || workers == 0)
        return -1;

    sched->worker_count = workers;

    sched->workers = calloc(workers, sizeof(img_worker_t));
    if (!sched->workers)
        return -1;

    img_mpmc_init(&sched->global_queue, 1024);

    for (uint32_t i = 0; i < workers; i++)
    {
        /*
         * Pass sched so workers can steal across queues.
         * ctx is NULL here — scheduler callers must set
         * sched->workers[i].ctx after init if needed,
         * OR pass a pre-built ctx array into a higher-level init.
         *
         * img_worker_init sets scheduler BEFORE pthread_create
         * so the thread never sees a NULL scheduler.
         */
        img_worker_init(&sched->workers[i], i, sched, NULL);
    }

    return 0;
}
