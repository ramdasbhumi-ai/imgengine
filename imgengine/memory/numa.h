/* memory/numa.h */
#ifndef IMGENGINE_MEMORY_NUMA_H
#define IMGENGINE_MEMORY_NUMA_H

#include <stddef.h>

void *img_numa_alloc_onnode(size_t size, int node);

#endif