// /* runtime/backpressure.c */
// #include "runtime/runtime.h"
// #include "runtime/queue_internal.h" // CRITICAL: Allows access to struct members

// bool img_backpressure_check(img_queue_t *q)
// {
//     if (!q)
//         return true; // Fail-safe: assume full if NULL

//     // L7 Logic: Atomic Acquire ensures we see the latest memory updates
//     // from both the Producer and Consumer threads.
//     uint32_t h = __atomic_load_n(&q->head, __ATOMIC_ACQUIRE);
//     uint32_t t = __atomic_load_n(&q->tail, __ATOMIC_ACQUIRE);

//     uint32_t usage = (t - h) & q->mask;

//     // Threshold check (90% capacity)
//     return usage > (uint32_t)(q->capacity * 0.9);
// }

// /* runtime/backpressure.c (Updated) */
// #include "runtime/runtime.h"
// #include "runtime/queue_internal.h"
// #include "observability/metrics.h" // Added

// bool img_backpressure_check(img_queue_t *q)
// {
//     uint32_t h = __atomic_load_n(&q->head, __ATOMIC_ACQUIRE);
//     uint32_t t = __atomic_load_n(&q->tail, __ATOMIC_ACQUIRE);
//     uint32_t usage = (t - h) & q->mask;

//     bool dropped = usage > (uint32_t)(q->capacity * 0.9);

//     if (dropped) {
//         img_metrics_inc(&g_metrics.backpressure_drops);
//     }

//     return dropped;
// }

/* runtime/backpressure.c */
#include "runtime/runtime.h"
#include "runtime/queue_internal.h"
#include "observability/metrics.h"

/**
 * @brief Checks if the SPSC queue is nearing saturation (90%).
 *
 * Logic: (Tail - Head) & Mask calculates the current occupancy.
 * If usage > 90%, we signal a drop and atomically update the metrics.
 */
bool img_backpressure_check(img_queue_t *q)
{
    // 1. Fail-safe: A NULL queue is a dead queue.
    if (__builtin_expect(!q, 0))
        return true;

    // 2. L7 Atomic Load: Acquire ensures we see the latest state
    // across CPU cache lines without a Mutex stall.
    uint32_t h = __atomic_load_n(&q->head, __ATOMIC_ACQUIRE);
    uint32_t t = __atomic_load_n(&q->tail, __ATOMIC_ACQUIRE);

    // 3. Distance calculation (Handles ring-buffer wrap-around)
    uint32_t usage = (t - h) & q->mask;

    // 4. Threshold Logic: 90% capacity limit
    bool dropped = usage > (uint32_t)(q->capacity * 0.9);

    // 5. Metric Increment: Only happens on the "Cold Path" of a drop.
    if (__builtin_expect(dropped, 0))
    {
        img_metrics_inc(&g_metrics.backpressure_drops);
    }

    return dropped;
}
