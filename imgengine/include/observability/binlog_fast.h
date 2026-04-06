// include/observability/binlog_fast.h

#ifndef IMGENGINE_BINLOG_FAST_H
#define IMGENGINE_BINLOG_FAST_H

#include "observability/binlog.h"

// global instance
extern img_binlog_t g_binlog;

// 🔥 HOT PATH MACROS (NO FUNCTION CALL OVERHEAD)
#define IMG_LOG(event, a0, a1, a2)        \
    do                                    \
    {                                     \
        img_binlog_write(&g_binlog,       \
                         (event),         \
                         (uint64_t)(a0),  \
                         (uint64_t)(a1),  \
                         (uint64_t)(a2)); \
    } while (0)

// ultra-fast variant (no cast)
#define IMG_LOG_FAST(e, a0, a1, a2) \
    img_binlog_write(&g_binlog, e, a0, a1, a2)

#endif