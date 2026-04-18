// ./src/api/api_shutdown_workers.c
#include "api/api_shutdown_internal.h"

#include "runtime/worker.h"

void img_api_shutdown_workers(img_engine_t *engine)
{
    if (!engine)
        return;

    for (uint32_t i = 0; i < engine->worker_count; i++)
    {
        img_worker_stop(&engine->workers[i]);
        img_worker_join(&engine->workers[i]);
        img_queue_destroy(engine->workers[i].queue);
        engine->workers[i].queue = NULL;
    }
}
