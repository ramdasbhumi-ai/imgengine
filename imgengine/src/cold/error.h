/* src/cold/error.h */
#ifndef IMGENGINE_COLD_ERROR_H
#define IMGENGINE_COLD_ERROR_H

#include "api/v1/img_error.h"

const char *img_error_to_string(img_result_t res);
void img_log_error(img_result_t res, const char *context);

#endif