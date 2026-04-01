/* src/hot/pixel_ops.h */
#include <stdint.h>

// Kernel-grade: Force inline to eliminate call stack overhead
static inline uint8_t pixel_lerp(uint8_t a, uint8_t b, uint8_t weight)
{
    // Fast linear interpolation using bit-shifting (No floating point)
    return (uint8_t)(a + ((weight * (b - a)) >> 8));
}

// 64-byte aligned memory copy for SIMD buffers
static inline void pixel_copy_aligned(void *restrict dst, const void *restrict src, size_t n)
{
    __builtin_memcpy(__builtin_assume_aligned(dst, 64),
                     __builtin_assume_aligned(src, 64), n);
}
