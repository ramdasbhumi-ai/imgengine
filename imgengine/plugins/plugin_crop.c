
/* plugins/plugin_crop.c */
#include "plugins/plugin_internal.h"

typedef struct
{
    uint32_t x, y, w, h;
} crop_params_t;

void plugin_crop_single(img_ctx_t *ctx, img_buffer_t *buf, void *params)
{
    // L7 Security: Never trust the params pointer
    if (!params || !buf)
        return;

    crop_params_t *p = (crop_params_t *)params;

    // SECURITY: Bounds check against original buffer
    if (p->x + p->w > buf->width || p->y + p->h > buf->height)
        return;

    // KERNEL-GRADE: Zero-copy pointer arithmetic
    // We simply offset the starting pointer. No pixels are moved.
    buf->data += (p->y * buf->stride) + (p->x * buf->channels);

    // Fix: Match the struct field names (w and h)
    buf->width = p->w;
    buf->height = p->h;
}
