// ./src/api/api_init_workers.c
#include "api/api_init_internal.h"
int img_api_init_boot_workers(uint32_t workers)
{
    uint32_t initialized = 0;

    for (uint32_t i = 0; i < workers; i++)
    {
        if (img_api_init_prepare_worker(i) != 0)
        {
            img_api_init_rollback(initialized);
            return -1;
        }

        initialized++;
    }

    if (img_api_init_prepare_task_queue() != 0)
    {
        img_api_init_rollback(initialized);
        return -1;
    }

    return 0;
}
