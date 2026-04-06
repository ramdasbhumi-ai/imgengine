// // arch/x86_64/cpu_detect.c

// #include "arch/cpu_caps.h"
// #include <stddef.h>

// #if defined(__x86_64__)
// #include <cpuid.h>

// cpu_caps_t img_detect_cpu(void)
// {
//     unsigned int eax, ebx, ecx, edx;

//     // Check AVX2
//     if (__get_cpuid_max(0, NULL) >= 7)
//     {
//         __cpuid_count(7, 0, eax, ebx, ecx, edx);

//         if (ebx & (1 << 5)) // AVX2
//             return ARCH_AVX2;

//         if (ebx & (1 << 16)) // AVX512F
//             return ARCH_AVX512;
//     }

//     return ARCH_SCALAR;
// }
// #else

// cpu_arch_t img_detect_cpu(void)
// {
// #if defined(__aarch64__)
//     return ARCH_NEON;
// #else
//     return ARCH_SCALAR;
// #endif
// }

// #endif

// src/arch/x86_64/cpu_detect.c

#include "arch/cpu_caps.h"
#include <stddef.h>

#if defined(__x86_64__)
#include <cpuid.h>

cpu_caps_t img_cpu_detect_caps(void)
{
    cpu_caps_t caps = 0;

    unsigned int eax, ebx, ecx, edx;

    // Ensure CPUID leaf 7 is supported
    if (__get_cpuid_max(0, NULL) >= 7)
    {
        __cpuid_count(7, 0, eax, ebx, ecx, edx);

        // AVX2 (bit 5 of EBX)
        if (ebx & (1 << 5))
            caps |= CPU_CAP_AVX2;

        // AVX512F (bit 16 of EBX)
        if (ebx & (1 << 16))
            caps |= CPU_CAP_AVX512;
    }

    return caps;
}

#elif defined(__aarch64__)

cpu_caps_t img_cpu_detect_caps(void)
{
    // On ARM64, NEON is mandatory
    return CPU_CAP_NEON;
}

#else

cpu_caps_t img_cpu_detect_caps(void)
{
    // Fallback: no SIMD guarantees
    return 0;
}

#endif