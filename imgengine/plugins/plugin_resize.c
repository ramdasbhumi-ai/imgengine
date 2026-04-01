#include "plugins/plugin_internal.h"
#include "arch/arch_kernels.h"
#include "memory/slab.h"
#include "core/image.h"

typedef struct
{
    uint32_t target_w;
    uint32_t target_h;
} resize_params_t;

void plugin_resize_single(img_ctx_t *ctx, img_buffer_t *buf, void *params)
{
    if (!ctx || !buf || !params)
        return;

    resize_params_t *p = (resize_params_t *)params;

    // Allocate new slab (O(1))
    uint8_t *out_mem = img_slab_alloc(ctx->pool);
    if (!out_mem)
        return;

    img_buffer_t dst = img_buffer_create(
        out_mem,
        p->target_w,
        p->target_h,
        buf->channels);

    // Hardware dispatch (branch once)
    switch (ctx->cpu_caps)
    {
    case ARCH_AVX512:
        img_arch_avx512_resize(buf, &dst);
        break;
    case ARCH_AVX2:
        img_arch_avx2_resize(buf, &dst);
        break;
    case ARCH_NEON:
        img_arch_neon_resize(buf, &dst);
        break;
    default:
        img_arch_scalar_resize(buf, &dst);
        break;
    }

    // Swap buffers (zero-copy style)
    img_slab_free(ctx->pool, buf->data);
    *buf = dst;
}

void plugin_resize_batch(img_ctx_t *ctx, img_batch_t *batch, void *params)
{
    if (!ctx || !batch || !params)
        return;

    // AVX-512 batch kernel (8-way)
    img_arch_batch_resize(ctx, batch, params);
}