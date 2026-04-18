// ./src/api/api_init_reset_engine.c
#include "api/api_init_internal.h"
#include "memory/slab.h"

void img_api_init_reset_engine(void)
{
    if (g_engine.global_pool)
    {
        img_slab_destroy(g_engine.global_pool);
        g_engine.global_pool = NULL;
    }

    img_api_init_reset_worker_state();
    img_api_init_reset_task_state();
    img_api_init_reset_engine_state();
}
