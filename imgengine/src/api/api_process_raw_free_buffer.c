// ./src/api/api_process_raw_free_buffer.c
#include "api/api_process_raw_internal.h"
#include "memory/slab.h"

void img_api_process_raw_free_buffer(
    img_engine_t *engine,
    img_buffer_t *buf)
{
    if (engine && buf && buf->data)
        img_slab_free(engine->global_pool, buf->data);
}
