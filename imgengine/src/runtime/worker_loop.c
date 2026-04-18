// ./src/runtime/worker_loop.c
#include "runtime/worker_internal.h"
#include "runtime/worker.h"
#include "runtime/scheduler.h"
#include "hot/pipeline_exec.h"
#include "observability/binlog.h"

#include <immintrin.h>

extern void img_binlog_set_cpu(uint32_t cpu);

void *img_worker_loop(void *arg)
{
    img_worker_t *w = (img_worker_t *)arg;

    img_binlog_set_cpu(w->id);

    while (__builtin_expect(w->running, 1))
    {
        img_task_t *task = (img_task_t *)img_queue_pop(w->queue);

        /*
         * STEAL: only attempt if scheduler is wired.
         * w->scheduler is set before pthread_create (see init below)
         * so this check is purely defensive — should never be NULL.
         */
        if (!task && w->scheduler)
            task = img_scheduler_steal(w->scheduler, w->id);

        if (!task)
        {
            _mm_pause();
            continue;
        }

        /*
         * Guard: ctx must be set. If NULL, skip task — don't crash.
         * In normal operation ctx is always set before thread starts.
         */
        if (__builtin_expect(!w->ctx, 0))
        {
            task->status = -1;
            continue;
        }

        img_pipeline_execute_hot(
            w->ctx,
            task->pipeline,
            task->buffer);

        task->status = 0;
    }

    return NULL;
}
