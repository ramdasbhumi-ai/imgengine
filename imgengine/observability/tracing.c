#include "observability/tracing.h"
#include <stdlib.h>
#include <time.h>

typedef struct
{
    const char *name;
    uint64_t start;
    uint64_t end;
} span_t;

static uint64_t now_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

img_span_t img_trace_span_start(const char *name)
{
    span_t *s = malloc(sizeof(span_t));
    s->name = name;
    s->start = now_ns();
    return (img_span_t)s;
}

void img_trace_span_end(img_span_t span)
{
    span_t *s = (span_t *)span;
    s->end = now_ns();

    // Future: push to async exporter
}