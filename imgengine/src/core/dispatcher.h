// /* src/core/dispatcher.h */
// #ifndef IMGENGINE_CORE_DISPATCHER_H
// #define IMGENGINE_CORE_DISPATCHER_H

// #include "arch/cpu_caps.h"

// /**
//  * Detect CPU SIMD capabilities at runtime
//  */
// cpu_arch_t img_dispatch_detect_cpu(void);

// cpu_arch_t img_detect_cpu(void);

// #endif

#ifndef IMGENGINE_CORE_DISPATCHER_H
#define IMGENGINE_CORE_DISPATCHER_H

#include "arch/cpu_caps.h"

cpu_arch_t img_detect_cpu(void);

#endif