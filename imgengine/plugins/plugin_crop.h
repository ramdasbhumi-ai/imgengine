/* plugins/plugin_crop.h */
#ifndef IMGENGINE_PLUGIN_CROP_H
#define IMGENGINE_PLUGIN_CROP_H

#include "core/context.h"
#include "core/image.h"

// Zero-copy crop
void plugin_crop_single(img_ctx_t *ctx, img_buffer_t *buf, void *params);

#endif