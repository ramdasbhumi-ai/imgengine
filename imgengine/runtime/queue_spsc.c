/* runtime/queue_spsc.c */
#include "runtime/runtime.h"
#include <stdlib.h>

typedef struct img_queue
{
    void **buffer;
    uint32_t capacity;
    uint32_t mask;
    alignas(64) uint32_t head; // Consumer side
    alignas(64) uint32_t tail; // Producer side
} img_queue_t;

img_queue_t *img_queue_create(uint32_t power_of_two)
{
    img_queue_t *q = malloc(sizeof(img_queue_t));
    q->capacity = 1 << power_of_two;
    q->mask = q->capacity - 1;
    q->buffer = calloc(q->capacity, sizeof(void *));
    q->head = 0;
    q->tail = 0;
    return q;
}

bool img_queue_push(img_queue_t *q, void *data)
{
    uint32_t t = q->tail;
    uint32_t next = (t + 1) & q->mask;

    // Check if full (using acquire to see consumer's head)
    if (next == __atomic_load_n(&q->head, __ATOMIC_ACQUIRE))
        return false;

    q->buffer[t] = data;
    __atomic_store_n(&q->tail, next, __ATOMIC_RELEASE);
    return true;
}

void *img_queue_pop(img_queue_t *q)
{
    uint32_t h = q->head;
    if (h == __atomic_load_n(&q->tail, __ATOMIC_ACQUIRE))
        return NULL;

    void *data = q->buffer[h];
    __atomic_store_n(&q->head, (h + 1) & q->mask, __ATOMIC_RELEASE);
    return data;
}
