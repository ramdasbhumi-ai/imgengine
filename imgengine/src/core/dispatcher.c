/* src/core/dispatcher.c */
#include "core/dispatcher.h"
#include <cpuid.h>

cpu_arch_t img_get_cpu_arch(void)
{
    unsigned int eax, ebx, ecx, edx;

    // Check for AVX-512 (Google/SaaS Standard)
    if (__get_cpuid_count(7, 0, &eax, &ebx, &ecx, &edx))
    {
        if (ebx & (1 << 16))
            return ARCH_AVX512; // AVX512F bit
        if (ebx & (1 << 5))
            return ARCH_AVX2; // AVX2 bit
    }

    // Fallback to Scalar for older hardware
    return ARCH_SCALAR;
}

// In a real implementation, add __builtin_cpu_supports("neon") for ARM
