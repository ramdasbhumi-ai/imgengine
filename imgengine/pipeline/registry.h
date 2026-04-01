/* pipeline/registry.h */
#ifndef IMGENGINE_PIPELINE_REGISTRY_H
#define IMGENGINE_PIPELINE_REGISTRY_H

#include "core/dispatcher.h"

/**
 * @brief Maps hardware-specific kernels (AVX/NEON/Scalar)
 * into the global jump table based on detected CPU arch.
 */
void img_registry_init_hardware(cpu_arch_t arch);

#endif
