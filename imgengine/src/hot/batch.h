/* src/hot/batch.h */
#ifndef IMGENGINE_HOT_BATCH_H
#define IMGENGINE_HOT_BATCH_H

#include "core/image.h"
#include "core/context.h"

#define BATCH_SIZE 8

typedef struct
{
    img_buffer_t *buffers[BATCH_SIZE];
    uint32_t count;
} img_batch_t;

#endif
