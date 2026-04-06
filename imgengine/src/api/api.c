// src/api/api.c

#include "api/v1/img_api.h"
#include "core/context_internal.h"
#include "runtime/worker.h"
#include "memory/slab.h"
#include "arch/cpu_caps.h"
#include "pipeline/jump_table.h"
#include "pipeline/hardware_registry.h"

#include <string.h>

static img_mpmc_queue_t g_task_queue;
// 🔥 STATIC ENGINE
static img_engine_t g_engine;
static img_worker_t g_workers[64];

img_engine_t *img_api_init(uint32_t workers)
{

    img_mpmc_init(&g_task_queue, 1024);

    if (workers == 0 || workers > 64)
        return NULL;

    memset(&g_engine, 0, sizeof(g_engine));

    g_engine.worker_count = workers;
    g_engine.workers = g_workers;

    // 🔥 CPU DETECT
    g_engine.caps = img_cpu_detect_caps();

    // 🔥 GLOBAL POOL (boundary alloc OK)
    g_engine.global_pool = img_slab_create(
        64 * 1024 * 1024,
        64 * 1024);

    // 🔥 INIT DISPATCH
    img_jump_table_init(g_engine.caps);
    img_hw_register_kernels(g_engine.caps);

    // 🔥 INIT WORKERS
    for (uint32_t i = 0; i < workers; i++)
    {
        img_worker_init(&g_workers[i], i);
        g_workers[i].queue = &g_task_queue;
    }

    return &g_engine;
}

// somewhere in API
void img_encoded_free(uint8_t *ptr)
{
    tjFree(ptr);
}

// 5. TASK SUBMISSION (PRODUCER SIDE)
// ✅ New API

int img_submit_task(img_task_t *task)
{
    return img_mpmc_push(&g_task_queue, task);
}

void img_api_shutdown(img_engine_t *engine)
{
    if (!engine)
        return;

    for (uint32_t i = 0; i < engine->worker_count; i++)
    {
        img_worker_stop(&engine->workers[i]);
        img_worker_join(&engine->workers[i]);
    }

    img_slab_destroy(engine->global_pool);
}
