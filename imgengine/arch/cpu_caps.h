// /* include/arch/cpu_caps.h */
// #ifndef IMGENGINE_ARCH_CPU_CAPS_H
// #define IMGENGINE_ARCH_CPU_CAPS_H

// #include <stdint.h>

// typedef enum
// {
//     ARCH_SCALAR = 0,
//     ARCH_AVX2,
//     ARCH_AVX512,
//     ARCH_NEON
// } cpu_arch_t;

// /**
//  * @brief Detect CPU capabilities at runtime
//  * Returns best available SIMD level
//  */
// static inline cpu_arch_t img_detect_cpu(void)
// {
// #if defined(ARCH_X86_64)

//     // Minimal detection (safe fallback version)
//     // You can later upgrade to full CPUID

// #if defined(__AVX512F__)
//     return ARCH_AVX512;
// #elif defined(__AVX2__)
//     return ARCH_AVX2;
// #else
//     return ARCH_SCALAR;
// #endif

// #elif defined(ARCH_AARCH64)

//     return ARCH_NEON;

// #else

//     return ARCH_SCALAR;

// #endif
// }

// #endif

/* include/arch/cpu_caps.h */
#ifndef IMGENGINE_ARCH_CPU_CAPS_H
#define IMGENGINE_ARCH_CPU_CAPS_H

#include <stdint.h>

typedef enum
{
    ARCH_SCALAR = 0,
    ARCH_AVX2,
    ARCH_AVX512,
    ARCH_NEON
} cpu_arch_t;

/**
 * @brief Detect CPU capabilities (implemented in dispatcher.c)
 */
cpu_arch_t img_detect_cpu(void);

#endif