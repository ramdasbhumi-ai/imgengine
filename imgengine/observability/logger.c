/* observability/logger.c */
#include "observability/logger.h"
#include "runtime/queue_spsc.h"
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

static img_queue_t *g_log_queue = NULL;
static pthread_t g_log_thread;
static volatile bool g_log_active = true;

// Cold Path: The thread that actually touches the slow Disk/TTY
void *img_log_drain_thread(void *arg)
{
    FILE *f = (FILE *)arg;
    while (g_log_active)
    {
        char *msg = (char *)img_queue_pop(g_log_queue);
        if (msg)
        {
            fprintf(f, "%s\n", msg);
            fflush(f);
            free(msg); // Free the message allocated by the worker
        }
        else
        {
            __builtin_ia32_pause(); // Save power when no logs
        }
    }
    return NULL;
}

void img_log_write(img_log_level_t level, const char *fmt, ...)
{
    if (level < LOG_INFO)
        return; // Skip debug in prod

    char *buffer = malloc(512); // Allocated for the queue
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, 512, fmt, args);
    va_end(args);

    // KERNEL-GRADE: Push to queue. If full, we drop the log, NOT the image.
    if (!img_queue_push(g_log_queue, buffer))
    {
        free(buffer);
    }
}
