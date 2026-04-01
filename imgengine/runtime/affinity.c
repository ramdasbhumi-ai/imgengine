/* runtime/affinity.c */
#define _GNU_SOURCE
#include "runtime/affinity.h"
#include <pthread.h>
#include <sched.h>

void img_affinity_set(uint32_t cpu_id)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu_id, &set);

    pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
}