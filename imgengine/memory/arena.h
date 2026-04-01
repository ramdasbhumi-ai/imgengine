/* memory/arena.h */
#ifndef IMGENGINE_MEMORY_ARENA_H
#define IMGENGINE_MEMORY_ARENA_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t *base;
    size_t size;
    size_t offset;
} img_arena_t;

/**
 * @brief Initialize arena
 */
void img_arena_init(img_arena_t *arena, void *memory, size_t size);

/**
 * @brief Allocate from arena (linear, no free)
 */
void *img_arena_alloc(img_arena_t *arena, size_t size);

/**
 * @brief Reset arena (O(1))
 */
void img_arena_reset(img_arena_t *arena);

#endif