/* observability/tracing.c */

#include "observability/tracing.h"
#include <stdlib.h>
#include <time.h>
#include <stdint.h> // ✅ FIX 1

// Internal struct (hidden from API)
typedef struct img_span_s
{
    const char *name;
    uint64_t start;
    uint64_t end;
} img_span_internal_t;

static uint64_t now_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

// ✅ MATCHES HEADER TYPE
img_span_t img_trace_span_start(const char *name)
{
    img_span_internal_t *s = malloc(sizeof(img_span_internal_t));
    if (!s)
        return NULL;

    s->name = name;
    s->start = now_ns();
    return (img_span_t)s; // safe cast
}

void img_trace_span_end(img_span_t span)
{
    if (!span)
        return;

    img_span_internal_t *s = (img_span_internal_t *)span;
    s->end = now_ns();

    // TODO: push to async exporter queue

    free(s); // ✅ prevent memory leak
}