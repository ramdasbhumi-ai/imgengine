/* runtime/affinity.c */
#define _GNU_SOURCE
#include <sched.h>
#include "runtime/runtime.h"

void img_affinity_set(uint32_t cpu_id)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);

    // Pin the current calling thread to the specified core
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}
