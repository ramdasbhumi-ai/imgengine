/* include/core/context_internal.h */
#ifndef IMGENGINE_CONTEXT_INTERNAL_H
#define IMGENGINE_CONTEXT_INTERNAL_H

#include "api/v1/img_core.h"
#include "memory/memory.h"
#include "arch/cpu_caps.h"

// The actual structure behind the 'img_engine_t' opaque handle
struct img_engine_s
{
    uint32_t worker_count;
    struct img_worker_s *workers; // Pointer to worker array
    img_slab_pool_t *global_pool; // Shared or per-worker slabs
    cpu_caps_t caps;              // Detected hardware flags
};

// Internal Context (Per-Thread)
typedef struct img_ctx
{
    uint32_t thread_id;
    img_slab_pool_t *local_pool;
    img_arena_t *scratch_arena;
    cpu_caps_t caps;
} img_ctx_t;

#endif
