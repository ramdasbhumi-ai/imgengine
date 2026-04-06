// include/core/buffer.h

#ifndef IMGENGINE_BUFFER_H
#define IMGENGINE_BUFFER_H

#include <stdint.h>

typedef struct img_buffer
{
    uint8_t *data;

    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint32_t stride;

} img_buffer_t;

#endif