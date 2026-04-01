/* observability/metrics.c */
#include "observability/metrics.h"
#include <stdio.h>
#include <stdbool.h>

// Global metrics scoreboard
img_metrics_t g_metrics = {0};

void img_metrics_init(void)
{
    __atomic_store_n(&g_metrics.min_latency, UINT64_MAX, __ATOMIC_RELAXED);
    __atomic_store_n(&g_metrics.max_latency, 0, __ATOMIC_RELAXED);

    // Note: slab_total should be set during engine init based on config
}

// ... rest of your existing img_metrics_inc and img_metrics_observe_latency ...

void img_metrics_export_prometheus(char *buffer, size_t size)
{
    uint64_t total_req = __atomic_load_n(&g_metrics.total_requests, __ATOMIC_RELAXED);
    uint64_t avg = (total_req > 0) ? (g_metrics.total_cycles / total_req) : 0;

    // 1. Write the first block
    int n = snprintf(buffer, size,
                     "img_latency_cycles_avg %lu\n"
                     "img_latency_cycles_max %lu\n"
                     "img_requests_total %lu\n"
                     "img_backpressure_drops %lu\n",
                     avg, g_metrics.max_latency, total_req, g_metrics.backpressure_drops);

    // 2. Append the second block safely
    if (n > 0 && (size_t)n < size)
    {
        uint64_t s_total = __atomic_load_n(&g_metrics.slab_total, __ATOMIC_RELAXED);
        uint64_t s_in_use = __atomic_load_n(&g_metrics.slab_in_use, __ATOMIC_RELAXED);

        snprintf(buffer + n, size - n,
                 "img_slab_total_blocks %lu\n"
                 "img_slab_in_use %lu\n"
                 "img_slab_utilization_percent %.2f\n",
                 s_total,
                 s_in_use,
                 (s_total > 0) ? ((float)s_in_use / s_total) * 100.0f : 0.0f);
    }
}
