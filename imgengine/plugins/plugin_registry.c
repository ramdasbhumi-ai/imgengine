/* plugins/registry.c */
#include "plugins/plugin_internal.h"
#include "pipeline/jump_table.h"

// Prototypes for static plugins
extern void plugin_resize_single(img_ctx_t *, img_buffer_t *, void *);
extern void plugin_crop_single(img_ctx_t *, img_buffer_t *, void *);

void img_plugins_init_all()
{
    // Map Opcodes to Function Pointers
    img_pipeline_register_op(OP_RESIZE, plugin_resize_single, plugin_resize_single);
    img_pipeline_register_op(OP_CROP, plugin_crop_single, NULL);

    // PDF and Bleed are registered similarly...
}
