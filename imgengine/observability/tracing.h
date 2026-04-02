/* observability/tracing.h */
#ifndef IMGENGINE_OBSERVABILITY_TRACING_H
#define IMGENGINE_OBSERVABILITY_TRACING_H

#include <stdint.h>
#include <stdbool.h>

typedef struct img_span_s *img_span_t;

/**
 * @brief Starts a new OpenTelemetry span.
 * @param name The operation name (e.g., "SIMD_RESIZE").
 * @return A handle to the span.
 */
img_span_t img_trace_span_start(const char *name);

/**
 * @brief Adds an attribute (Metadata) to the current span.
 */
void img_trace_add_attr(img_span_t span, const char *key, const char *val);

/**
 * @brief Ends the span and pushes it to the Async Ring Buffer.
 */
void img_trace_span_end(img_span_t span);

#endif
