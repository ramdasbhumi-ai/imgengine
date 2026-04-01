/* memory/numa.c */
#include "memory/numa.h"

#ifdef __linux__
#include <numa.h>
#endif

void *img_numa_alloc_onnode(size_t size, int node)
{
#ifdef __linux__
    return numa_alloc_onnode(size, node);
#else
    (void)node;
    return NULL;
#endif
}