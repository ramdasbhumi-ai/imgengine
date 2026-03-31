/* api/v1/img_types.h */
#ifndef IMGENGINE_API_TYPES_H
#define IMGENGINE_API_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef enum
{
    IMG_FMT_JPEG = 0,
    IMG_FMT_PNG = 1,
    IMG_FMT_RAW = 2
} img_format_t;

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    img_format_t format;
} img_meta_t;

#endif
