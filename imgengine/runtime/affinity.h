/* runtime/affinity.h */
#ifndef IMGENGINE_RUNTIME_AFFINITY_H
#define IMGENGINE_RUNTIME_AFFINITY_H

#include <stdint.h>

/**
 * Pin thread to CPU core
 */
void img_affinity_set(uint32_t cpu_id);

#endif