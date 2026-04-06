// tools/binlog_decode.c

#include <stdio.h>
#include "observability/binlog.h"

void decode(img_log_entry_t *e)
{
    switch (e->event_id)
    {
    case IMG_LOG_LATENCY:
        printf("[LAT] %lu cycles w=%lu h=%lu\n",
               e->arg0, e->arg1, e->arg2);
        break;

    case IMG_LOG_ERROR:
        printf("[ERR] code=%lu\n", e->arg0);
        break;
    }
}