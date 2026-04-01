/* pipeline/registry.c */
#include "pipeline/registry.h"
#include "pipeline/jump_table.h"

// Plugins
#include "plugins/plugin_resize.h"
#include "plugins/plugin_crop.h"

// Arch kernels
#include "arch/arch_kernels.h"

#define OP_RESIZE 0x01
#define OP_CROP 0x02

void img_registry_init_hardware(cpu_arch_t arch)
{
    switch (arch)
    {
    case ARCH_AVX512:
    case ARCH_AVX2:
        img_pipeline_register_op(OP_RESIZE, img_arch_avx2_resize, NULL);
        break;

    case ARCH_NEON:
        img_pipeline_register_op(OP_RESIZE, img_arch_neon_resize, NULL);
        break;

    default:
        img_pipeline_register_op(OP_RESIZE, img_arch_scalar_resize, NULL);
        break;
    }
}

void img_plugins_init_all()
{
    img_pipeline_register_op(OP_RESIZE, plugin_resize_single, NULL);
    img_pipeline_register_op(OP_CROP, plugin_crop_single, NULL);
}