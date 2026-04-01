/* runtime/queue_spsc.h */
#ifndef IMGENGINE_RUNTIME_QUEUE_SPSC_H
#define IMGENGINE_RUNTIME_QUEUE_SPSC_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Opaque handle for the Single-Producer Single-Consumer queue.
 */
typedef struct img_queue img_queue_t;

/**
 * @brief Creates a lock-free queue with capacity 2^power_of_two.
 */
img_queue_t *img_queue_create(uint32_t power_of_two);

/**
 * @brief Non-blocking push. Returns false if the queue is full.
 */
bool img_queue_push(img_queue_t *q, void *data);

/**
 * @brief Non-blocking pop. Returns NULL if the queue is empty.
 */
void *img_queue_pop(img_queue_t *q);

#endif
