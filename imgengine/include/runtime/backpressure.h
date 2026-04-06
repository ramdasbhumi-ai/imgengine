/* runtime/backpressure.h */
#ifndef IMGENGINE_RUNTIME_BACKPRESSURE_H
#define IMGENGINE_RUNTIME_BACKPRESSURE_H

#include <stdbool.h>
#include "runtime/queue_spsc.h"
#include "runtime/queue_mpmc.h"

/**
 * @brief Returns true if queue is overloaded
 */
// bool img_backpressure_check(img_queue_t *q);

static inline bool img_backpressure_check(img_mpmc_queue_t *q)
{
    size_t head = atomic_load(&q->head);
    size_t tail = atomic_load(&q->tail);

    return (tail - head) > (q->size * 3 / 4);
}

#endif