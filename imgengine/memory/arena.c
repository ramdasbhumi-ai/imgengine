/* memory/arena.c */
#include "memory.h"

struct img_arena
{
    uint8_t *data;
    size_t capacity;
    size_t offset;
};

void *img_arena_alloc(img_arena_t *arena, size_t size)
{
    size_t aligned_size = (size + 7) & ~7; // 8-byte alignment
    if (arena->offset + aligned_size > arena->capacity)
        return NULL;

    void *ptr = &arena->data[arena->offset];
    arena->offset += aligned_size;
    return ptr;
}
