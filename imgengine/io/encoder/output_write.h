/* io/encoder/output_writer.h */
#ifndef IMGENGINE_IO_ENCODER_H
#define IMGENGINE_IO_ENCODER_H

#include "api/v1/img_error.h"
#include "core/image.h"

img_result_t img_encode_to_mem(img_buffer_t *buf, uint8_t **out_ptr, size_t *out_size);

#endif
