/* plugins/plugin_resize.c */
#include "plugins/plugin_internal.h"
#include "arch/arch_kernels.h"

typedef struct
{
    uint32_t target_w;
    uint32_t target_h;
} resize_params_t;

// Single Image Execution
void plugin_resize_single(img_ctx_t *ctx, img_buffer_t *buf, void *params)
{
    resize_params_t *p = (resize_params_t *)params;

    // Dispatch to the best hardware kernel detected at boot
    if (ctx->cpu_caps == ARCH_AVX512)
    {
        img_arch_avx512_resize(buf, p->target_w, p->target_h);
    }
    else if (ctx->cpu_caps == ARCH_NEON)
    {
        img_arch_neon_resize(buf, p->target_w, p->target_h);
    }
    else
    {
        img_arch_scalar_resize(buf, p->target_w, p->target_h);
    }
}

// Batch Image Execution (SIMD-Interleaved)
void plugin_resize_batch(img_ctx_t *ctx, img_batch_t *batch, void *params)
{
    // Calls the batch-aware kernels from Phase 9
    img_arch_batch_resize(ctx, batch, (resize_params_t *)params);
}
