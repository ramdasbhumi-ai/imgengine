/* src/core/context.h */
#ifndef IMGENGINE_CORE_CONTEXT_H
#define IMGENGINE_CORE_CONTEXT_H

#include <stdint.h>
#include <stddef.h>
#include "memory/slab.h"
#include "src/core/dispatcher.h"

/**
 * @brief Thread-local execution context.
 * One context per physical CPU core to ensure zero lock contention.
 */
typedef struct
{
    uint32_t worker_id;    // Unique ID for the pinned core
    img_slab_pool_t *pool; // The O(1) Slab Allocator for this thread
    cpu_arch_t cpu_caps;   // Hardware flags (AVX512/AVX2/NEON)
} img_ctx_t;

/**
 * @brief Bootstraps a new context and its dedicated memory pool.
 */
img_ctx_t *img_ctx_init(uint32_t worker_id, size_t slab_size, size_t num_slabs);

/**
 * @brief Tears down the context and returns all HugePages to the OS.
 */
void img_ctx_destroy(img_ctx_t *ctx);

#endif
