// src/observability/binlog.c
//
// _GNU_SOURCE MUST be the first line before any #include.
// clock_gettime + CLOCK_MONOTONIC are POSIX extensions hidden
// by glibc unless _GNU_SOURCE is defined BEFORE <time.h> is seen.
// Any header that includes <time.h> transitively will lock in
// the non-POSIX view if _GNU_SOURCE hasn't been set yet.
//
// Rule: every .c file that uses POSIX/GNU extensions defines
// _GNU_SOURCE as its very first line. Not in headers — in .c files.
// Headers may guard with #ifndef _GNU_SOURCE but cannot guarantee
// ordering against transitive includes.

#define _GNU_SOURCE

#include "observability/binlog.h"
#include "core/time.h"

#include <stdlib.h>
#include <string.h>

/*
 * Global binlog instance.
 * Single source of truth. Declared extern in binlog_fast.h.
 * Sharded per-CPU — no false sharing.
 */
img_binlog_t g_binlog;

/*
 * TLS CPU ID — set once per thread by img_binlog_set_cpu().
 * Avoids sched_getcpu() syscall on every write.
 */
static __thread uint32_t tls_cpu_id = 0;

void img_binlog_set_cpu(uint32_t cpu)
{
    tls_cpu_id = cpu;
}

int img_binlog_init(
    img_binlog_t *log,
    uint32_t cpu_count,
    uint32_t power)
{
    if (!log || cpu_count == 0 || cpu_count > IMG_MAX_CPUS)
        return -1;

    memset(log, 0, sizeof(*log));

    log->cpu_count = cpu_count;

    uint32_t size = 1u << power;

    for (uint32_t i = 0; i < cpu_count; i++)
    {
        img_binlog_cpu_t *c = &log->cpu_logs[i];

        c->entries = aligned_alloc(
            64,
            sizeof(img_log_entry_t) * size);

        if (!c->entries)
            return -1;

        c->size = size;
        c->mask = size - 1;
        c->head = 0;
        c->tail = 0;
    }

    return 0;
}

void img_binlog_destroy(img_binlog_t *log)
{
    if (!log)
        return;

    for (uint32_t i = 0; i < log->cpu_count; i++)
        free(log->cpu_logs[i].entries);
}

/*
 * img_binlog_write()
 *
 * Lock-free, atomic-free ring buffer write.
 * SPSC per CPU — only the pinned worker thread writes to its slot.
 * Consumer (flush thread) reads head. No CAS needed.
 *
 * Drop policy: if ring is full, drop the entry silently.
 * Never blocks. Never allocates.
 */
void img_binlog_write(
    img_binlog_t *log,
    uint32_t event,
    uint64_t arg0,
    uint64_t arg1,
    uint64_t arg2)
{
    uint32_t cpu = tls_cpu_id;

    if (__builtin_expect(cpu >= log->cpu_count, 0))
        return;

    img_binlog_cpu_t *c = &log->cpu_logs[cpu];

    uint32_t tail = c->tail;
    uint32_t next = (tail + 1) & c->mask;

    if (__builtin_expect(next == c->head, 0))
        return; /* full — drop, never block */

    img_log_entry_t *e = &c->entries[tail];

    e->timestamp = img_now_ns();
    e->event_id = event;
    e->cpu = cpu;
    e->arg0 = arg0;
    e->arg1 = arg1;
    e->arg2 = arg2;

    c->tail = next;
}