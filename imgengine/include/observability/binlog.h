// include/observability/binlog.h

#ifndef IMGENGINE_BINLOG_H
#define IMGENGINE_BINLOG_H

#include <stdint.h>
#include <stdatomic.h>

// ================================
// EVENT IDS (STABLE ABI)
// ================================
typedef enum
{
    IMG_LOG_OP_START = 1,
    IMG_LOG_OP_END,
    IMG_LOG_LATENCY,
    IMG_LOG_ERROR,
    IMG_LOG_DROP
} img_log_event_t;

// ================================
// LOG ENTRY (CACHE ALIGNED)
// ================================
typedef struct __attribute__((aligned(64)))
{
    uint64_t timestamp;

    uint32_t event_id;
    uint32_t cpu;

    uint64_t arg0;
    uint64_t arg1;
    uint64_t arg2;

} img_log_entry_t;

// ================================
// RING BUFFER
// ================================
typedef struct
{
    img_log_entry_t *entries;
    uint32_t size;
    uint32_t mask;

    _Atomic uint32_t head;
    _Atomic uint32_t tail;

} img_binlog_t;

// ================================
// API
// ================================
int img_binlog_init(img_binlog_t *log, uint32_t power);
void img_binlog_destroy(img_binlog_t *log);

int img_binlog_write(
    img_binlog_t *log,
    uint32_t event,
    uint64_t arg0,
    uint64_t arg1,
    uint64_t arg2);

#endif