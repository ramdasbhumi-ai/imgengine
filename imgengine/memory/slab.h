// /* memory/slab.h */
// #ifndef IMGENGINE_MEMORY_SLAB_H
// #define IMGENGINE_MEMORY_SLAB_H

// #include <stddef.h>
// #include <stdint.h>

// /**
//  * @brief Opaque handle for a fixed-size block pool.
//  * Guaranteed O(1) allocation and zero fragmentation.
//  */
// typedef struct img_slab_pool img_slab_pool_t;

// /**
//  * @brief Initialize a slab pool on a specific NUMA node.
//  * @param block_size Size of each image buffer (e.g., 16MB for 4K).
//  * @param num_blocks Total buffers to pre-allocate.
//  * @param node NUMA node ID (-1 for current).
//  */
// img_slab_pool_t *img_slab_init(size_t block_size, size_t num_blocks, int node);

// /**
//  * @brief Pop a free block from the LIFO stack.
//  */
// void *img_slab_alloc(img_slab_pool_t *pool);

// /**
//  * @brief Return a block to the stack for reuse.
//  */
// void img_slab_free(img_slab_pool_t *pool, void *ptr);

// /**
//  * @brief Free the entire pool and HugePages.
//  */
// void img_slab_destroy(img_slab_pool_t *pool);

// #endif

/* memory/slab.h */
#ifndef IMGENGINE_SLAB_BRIDGE_H
#define IMGENGINE_SLAB_BRIDGE_H

// Redirect to your master memory header
#include "memory/memory.h"

#endif
