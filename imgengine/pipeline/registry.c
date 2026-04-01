
/* pipeline/registry.c */
#include "pipeline/pipeline.h"
#include "arch/arch_kernels.h"

void img_registry_init_hardware(cpu_arch_t arch)
{
    if (arch == ARCH_AVX512)
    {
        img_pipeline_register_op(OP_RESIZE, img_arch_avx512_resize, NULL);
    }
    else if (arch == ARCH_NEON)
    {
        img_pipeline_register_op(OP_RESIZE, img_arch_neon_resize, NULL);
    }
    else
    {
        img_pipeline_register_op(OP_RESIZE, img_arch_scalar_resize, NULL);
    }
}
