/* observability/profiler.h */
#ifndef IMGENGINE_OBSERVABILITY_PROFILER_H
#define IMGENGINE_OBSERVABILITY_PROFILER_H

#include <stdint.h>
#ifdef ARCH_X86_64
#include <x86intrin.h>
#endif

static inline uint64_t img_profiler_now(void)
{
#if defined(ARCH_X86_64)
    // Read Time-Stamp Counter (Intel/AMD)
    return __rdtsc();
#elif defined(ARCH_AARCH64)
    // Read Virtual Counter Register (ARM)
    uint64_t val;
    asm volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
#else
    return 0;
#endif
}

#endif
