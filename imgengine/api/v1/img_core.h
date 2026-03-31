/* api/v1/img_core.h */
#ifndef IMGENGINE_API_CORE_H
#define IMGENGINE_API_CORE_H

#include "img_types.h"
#include "img_error.h"

/**
 * @brief Opaque handle to the engine instance.
 * Hides all internal Slab and Worker details.
 */
typedef struct img_engine_s *img_engine_t;

/**
 * @brief Initialize the engine.
 * @param num_workers Number of physical cores to pin.
 * @return Handle to engine or NULL if Slab allocation fails.
 */
img_engine_t img_api_init(uint32_t num_workers);

/**
 * @brief The "Fast Path" execution.
 * Decodes, Resizes (SIMD), and Encodes in a single non-blocking pass.
 */
img_result_t img_api_process_fast(img_engine_t engine,
                                  const char *in_path,
                                  const char *out_path,
                                  uint32_t w, uint32_t h);

void img_api_shutdown(img_engine_t engine);

#endif
