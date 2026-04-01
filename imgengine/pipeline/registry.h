/* pipeline/registry.h */
#ifndef IMGENGINE_PIPELINE_REGISTRY_H
#define IMGENGINE_PIPELINE_REGISTRY_H

#include "arch/cpu_caps.h"

/**
 * Register hardware kernels based on CPU
 */
void img_registry_init_hardware(cpu_arch_t arch);

/**
 * Register all plugins
 */
void img_plugins_init_all(void);

#endif