/* security/bounds_check.h */
#ifndef IMGENGINE_SECURITY_BOUNDS_H
#define IMGENGINE_SECURITY_BOUNDS_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Ensures a SIMD operation (e.g., 64-byte write) stays within the Slab.
 * Force inline to keep the hot-path overhead near zero.
 */
static inline bool img_security_check_bounds(const void* ptr, size_t access_size, 
                                             const void* slab_base, size_t slab_size) {
    uintptr_t p = (uintptr_t)ptr;
    uintptr_t base = (uintptr_t)slab_base;
    return (p >= base) && (p + access_size <= base + slab_size);
}

#endif
