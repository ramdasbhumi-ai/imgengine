
/* observability/metrics.c */
#include "observability/metrics.h"
#include <stdio.h>

// Global metrics scoreboard
img_metrics_t g_metrics = {0};

void img_metrics_init(void)
{
    __atomic_store_n(&g_metrics.min_latency, UINT64_MAX, __ATOMIC_RELAXED);
    __atomic_store_n(&g_metrics.max_latency, 0, __ATOMIC_RELAXED);
}

void img_metrics_inc(uint64_t *counter)
{
    __atomic_add_fetch(counter, 1, __ATOMIC_RELAXED);
}

void img_metrics_observe_latency(uint64_t cycles)
{
    // 1. Update Global Max (Atomic CAS Loop)
    uint64_t current_max = __atomic_load_n(&g_metrics.max_latency, __ATOMIC_RELAXED);
    while (cycles > current_max &&
           !__atomic_compare_exchange_n(&g_metrics.max_latency, &current_max, cycles,
                                        false, __ATOMIC_RELAXED, __ATOMIC_RELAXED))
        ;

    // 2. Update Global Min (Atomic CAS Loop)
    uint64_t current_min = __atomic_load_n(&g_metrics.min_latency, __ATOMIC_RELAXED);
    while (cycles < current_min &&
           !__atomic_compare_exchange_n(&g_metrics.min_latency, &current_min, cycles,
                                        false, __ATOMIC_RELAXED, __ATOMIC_RELAXED))
        ;

    // 3. Simple Running Sum for P50 estimation
    __atomic_add_fetch(&g_metrics.total_cycles, cycles, __ATOMIC_RELAXED);
}

// void img_metrics_export_prometheus(char *buffer, size_t size)
// {
//     // Convert cycles to microseconds (Assuming 3GHz baseline or using profiler freq)
//     uint64_t total_req = __atomic_load_n(&g_metrics.total_requests, __ATOMIC_RELAXED);
//     uint64_t avg = (total_req > 0) ? (g_metrics.total_cycles / total_req) : 0;

//     snprintf(buffer, size,
//              "# HELP img_latency_cycles_avg Average CPU cycles per request\n"
//              "img_latency_cycles_avg %lu\n"
//              "# HELP img_latency_cycles_max Maximum CPU cycles observed\n"
//              "img_latency_cycles_max %lu\n"
//              "img_requests_total %lu\n"
//              "img_backpressure_drops %lu\n",
//              avg, g_metrics.max_latency, total_req, g_metrics.backpressure_drops);
// }

void img_metrics_export_prometheus(char *buffer, size_t size)
{

    // Convert cycles to microseconds (Assuming 3GHz baseline or using profiler freq)
    uint64_t total_req = __atomic_load_n(&g_metrics.total_requests, __ATOMIC_RELAXED);
    uint64_t avg = (total_req > 0) ? (g_metrics.total_cycles / total_req) : 0;

    // 1. Write the first block and track the length
    int n = snprintf(buffer, size,
                     "img_latency_cycles_avg %lu\n"
                     "img_latency_cycles_max %lu\n"
                     "img_requests_total %lu\n"
                     "img_backpressure_drops %lu\n",
                     avg, g_metrics.max_latency, total_req, g_metrics.backpressure_drops);

    // 2. Append the second block using pointer arithmetic (buffer + n)
    if (n > 0 && (size_t)n < size)
    {
        snprintf(buffer + n, size - n,
                 "img_slab_total_blocks %lu\n"
                 "img_slab_in_use %lu\n"
                 "img_slab_utilization_percent %.2f\n",
                 g_metrics.slab_total,
                 g_metrics.slab_in_use,
                 (g_metrics.slab_total > 0) ? ((float)g_metrics.slab_in_use / g_metrics.slab_total) * 100.0 : 0.0);
    }
}
