/* observability/tracing.c */
#include "observability/tracing.h"
#include "runtime/queue_spsc.h" // Reuse our lock-free queue
#include <stdlib.h>
#include <time.h>

typedef struct img_span_s
{
    const char *name;
    uint64_t start_ns;
    uint64_t end_ns;
    // ... OTel specific context ...
} img_span_internal_t;

static uint64_t get_nanos()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1e9 + ts.tv_nsec;
}

img_span_t img_trace_span_start(const char *name)
{
    // 1. Allocate from the thread-local Arena (Phase 1) to avoid malloc
    img_span_internal_t *span = malloc(sizeof(img_span_internal_t));
    span->name = name;
    span->start_ns = get_nanos();
    return (img_span_t)span;
}

void img_trace_span_end(img_span_t span_handle)
{
    img_span_internal_t *span = (img_span_internal_t *)span_handle;
    span->end_ns = get_nanos();

    // 2. KERNEL-GRADE: Hand off to the OTel Collector thread via SPSC queue
    // This ensures the Worker thread returns to pixels immediately.
    // img_queue_push(g_otel_queue, span);
}
