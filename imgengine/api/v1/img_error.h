/* api/v1/img_error.h */
#ifndef IMGENGINE_API_ERROR_H
#define IMGENGINE_API_ERROR_H

typedef enum
{
    IMG_SUCCESS = 0,
    IMG_ERR_NOMEM = -1,    // Slab exhausted or HugePage fail
    IMG_ERR_FORMAT = -2,   // Malicious or unsupported header
    IMG_ERR_HW_UNSUP = -3, // CPU lacks required SIMD (AVX2/AVX512)
    IMG_ERR_IO = -4,       // Disk/S3 read failure
    IMG_ERR_TIMEOUT = -5,  // SPSC Queue backpressure
    IMG_ERR_SECURITY = -6  // Bounds check violation
} img_result_t;

#endif
