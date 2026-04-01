/* src/core/image.h */
#ifndef IMGENGINE_CORE_IMAGE_H
#define IMGENGINE_CORE_IMAGE_H

#include <stdint.h>

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint32_t stride; // Bytes per row (including 64-byte padding)
    uint8_t *data;   // Pointer to the actual Slab memory
} img_buffer_t;

/**
 * @brief Logic to wrap raw slab memory into an image structure.
 */
img_buffer_t img_buffer_create(uint8_t *memory, uint32_t w, uint32_t h, uint32_t c);

#endif
