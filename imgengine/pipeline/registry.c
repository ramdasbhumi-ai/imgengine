
/* pipeline/registry.c */
#include "pipeline/pipeline.h"
#include "arch/arch_kernels.h"
#include "pipeline/registry.h" // Added

#include "plugins/plugin_internal.h"
#include "pipeline/jump_table.h"

void img_registry_init_hardware(cpu_arch_t arch)
{
    if (arch == ARCH_AVX512)
    {
        img_pipeline_register_op(OP_RESIZE, img_arch_avx2_resize, NULL);
    }
    else if (arch == ARCH_NEON)
    {
        img_pipeline_register_op(OP_RESIZE, img_arch_avx2_resize, NULL);
    }
    else
    {
        img_pipeline_register_op(OP_RESIZE, img_arch_scalar_resize, NULL);
    }
}

// Prototypes for static plugins - ensure these match the function signatures
extern void plugin_resize_single(img_ctx_t *, img_buffer_t *, void *);
extern void plugin_resize_batch(img_ctx_t *, img_batch_t *, void *); // Added
extern void plugin_crop_single(img_ctx_t *, img_buffer_t *, void *);

void img_plugins_init_all()
{
    // KERNEL-GRADE: Static mapping to the O(1) Jump Table
    img_pipeline_register_op(OP_RESIZE, plugin_resize_single, plugin_resize_batch);
    img_pipeline_register_op(OP_CROP, plugin_crop_single, NULL);

    // PDF and Bleed are registered similarly...
}
