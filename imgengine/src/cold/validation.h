/* src/cold/validation.h */
#ifndef IMGENGINE_COLD_VALIDATION_H
#define IMGENGINE_COLD_VALIDATION_H

#include "core/image.h"
#include "api/v1/img_error.h"

// Hard limits to prevent OOM/DoS
#define MAX_IMG_DIM 16384 

img_result_t img_validate_header(uint32_t w, uint32_t h, uint32_t channels);
bool img_validate_pipeline_safety(const img_pipeline_t* pipe);

#endif

