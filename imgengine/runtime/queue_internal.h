/* runtime/queue_internal.h */
#ifndef IMGENGINE_QUEUE_INTERNAL_H
#define IMGENGINE_QUEUE_INTERNAL_H

#include "runtime/runtime.h"

struct img_queue
{
    void **buffer;
    uint32_t capacity;
    uint32_t mask;
    alignas(64) uint32_t head;
    alignas(64) uint32_t tail;
};

#endif
