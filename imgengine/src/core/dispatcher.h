/* src/core/dispatcher.h */
#ifndef IMGENGINE_CORE_DISPATCHER_H
#define IMGENGINE_CORE_DISPATCHER_H

typedef enum
{
    ARCH_SCALAR = 0,
    ARCH_AVX2 = 1,
    ARCH_AVX512 = 2,
    ARCH_NEON = 3
} cpu_arch_t;

cpu_arch_t img_get_cpu_arch(void);

#endif
