/* memory/slab.c */
#include "memory.h"
#include <stdlib.h>

typedef struct slab_node
{
    struct slab_node *next;
} slab_node_t;

struct img_slab_pool
{
    void *raw_region;
    slab_node_t *free_list;
    size_t block_size;
    size_t total_size;
};

img_slab_pool_t *img_slab_init(size_t block_size, size_t num_blocks, int node)
{
    img_slab_pool_t *pool = malloc(sizeof(img_slab_pool_t));
    pool->block_size = (block_size + 63) & ~63; // Align to 64-byte
    pool->total_size = pool->block_size * num_blocks;

    // Allocate raw memory (HugePages + NUMA node logic)
    pool->raw_region = img_hugepage_alloc(pool->total_size);

    // Build internal LIFO freelist
    pool->free_list = (slab_node_t *)pool->raw_region;
    slab_node_t *curr = pool->free_list;
    for (size_t i = 0; i < num_blocks - 1; i++)
    {
        curr->next = (slab_node_t *)((uint8_t *)curr + pool->block_size);
        curr = curr->next;
    }
    curr->next = NULL;
    return pool;
}

void *img_slab_alloc(img_slab_pool_t *pool)
{
    if (!pool->free_list)
        return NULL; // Backpressure: Slab Full
    void *ptr = pool->free_list;
    pool->free_list = pool->free_list->next;
    return ptr;
}

void img_slab_free(img_slab_pool_t *pool, void *ptr)
{
    slab_node_t *node = (slab_node_t *)ptr;
    node->next = pool->free_list;
    pool->free_list = node;
}
