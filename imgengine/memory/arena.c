/* memory/arena.c */
#include "memory/arena.h"

void img_arena_init(img_arena_t* arena, void* memory, size_t size)
{
    arena->base = (uint8_t*)memory;
    arena->size = size;
    arena->offset = 0;
}

void* img_arena_alloc(img_arena_t* arena, size_t size)
{
    if (arena->offset + size > arena->size)
        return NULL;

    void* ptr = arena->base + arena->offset;
    arena->offset += size;

    return ptr;
}

void img_arena_reset(img_arena_t* arena)
{
    arena->offset = 0;
}