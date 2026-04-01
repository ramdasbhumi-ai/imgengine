/* observability/logger.h */
#ifndef IMGENGINE_OBSERVABILITY_LOGGER_H
#define IMGENGINE_OBSERVABILITY_LOGGER_H

#include <stdint.h>

typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO  = 1,
    LOG_WARN  = 2,
    LOG_ERROR = 3
} img_log_level_t;

/**
 * @brief Initialize the async logging thread.
 */
void img_log_init(const char* log_file);

/**
 * @brief Lock-free log submission.
 */
void img_log_write(img_log_level_t level, const char* fmt, ...);

void img_log_shutdown(void);

#endif
