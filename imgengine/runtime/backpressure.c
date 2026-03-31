/* runtime/backpressure.c */
#include "runtime/runtime.h"

bool img_backpressure_check(img_queue_t *q)
{
    uint32_t h = __atomic_load_n(&q->head, __ATOMIC_ACQUIRE);
    uint32_t t = __atomic_load_n(&q->tail, __ATOMIC_ACQUIRE);
    uint32_t usage = (t - h) & q->mask;

    // Fail if we are at >90% capacity
    return usage > (q->capacity * 0.9);
}
