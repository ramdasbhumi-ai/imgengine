// // /* plugins/plugin_resize.h */

// #ifndef IMGENGINE_PLUGIN_RESIZE_H
// #define IMGENGINE_PLUGIN_RESIZE_H

// #include "core/image.h"
// #include <stdint.h>

// typedef struct
// {
//     img_buffer_t *src; // 🔥 REQUIRED (input image)
//     uint32_t target_w;
//     uint32_t target_h;
// } resize_params_t;

// #endif

#ifndef IMGENGINE_PLUGIN_RESIZE_H
#define IMGENGINE_PLUGIN_RESIZE_H

#include <stdint.h>
#include "core/image.h"

typedef struct
{
    uint32_t target_w;
    uint32_t target_h;

    // 🔥 Precomputed FIXED-POINT scaling
    uint32_t scale_x; // Q16.16
    uint32_t scale_y;

    img_buffer_t *src;

} resize_params_t;

#endif