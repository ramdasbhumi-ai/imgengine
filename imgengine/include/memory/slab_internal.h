/* include/memory/slab_internal.h */
#ifndef IMGENGINE_SLAB_INTERNAL_H
#define IMGENGINE_SLAB_INTERNAL_H

#include <stddef.h>

typedef struct slab_block
{
    struct slab_block *next;
    uint8_t data[]; // Flexible array member for zero-overhead
} slab_block_t;

struct img_slab_pool
{
    slab_block_t *free_list;
    void *raw_mmap_ptr;
    size_t total_capacity;
    size_t block_size;
    int numa_node;
};

#endif
