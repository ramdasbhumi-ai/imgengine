// plugins/plugin_resize.c

#include "api/v1/img_plugin_api.h"
#include "core/buffer.h"
#include <stddef.h>

static void resize_single(img_ctx_t *ctx, img_buffer_t *buf, void *params)
{
    (void)ctx;
    (void)params;

    buf->width /= 2;
    buf->height /= 2;
}

static img_plugin_descriptor_t plugin = {
    .abi_version = IMG_PLUGIN_ABI_VERSION,
    .op_code = 1,
    .name = "resize",
    .capabilities = IMG_CAP_SINGLE | IMG_CAP_ZERO_COPY,
    .single_exec = resize_single,
    .batch_exec = NULL};

const img_plugin_descriptor_t *img_plugin_get_descriptor(void)
{
    return &plugin;
}