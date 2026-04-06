/* observability/tracing.c */

#define _GNU_SOURCE

#include "observability/tracing.h"
#include <time.h>

static inline uint64_t now_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

img_span_t img_trace_start(const char *name)
{
    img_span_t s;
    s.name = name;
    s.start = now_ns();
    s.end = 0;
    return s;
}

void img_trace_end(img_span_t *s)
{
    if (!s)
        return;

    s->end = now_ns();

    // future: push to lock-free ring buffer exporter
}