// // /* plugins/plugin_resize.c */
// // #include "plugins/plugin_internal.h"
// // #include "arch/arch_kernels.h"

// // typedef struct
// // {
// //     uint32_t target_w;
// //     uint32_t target_h;
// // } resize_params_t;

// // // Single Image Execution
// // void plugin_resize_single(img_ctx_t *ctx, img_buffer_t *buf, void *params)
// // {
// //     resize_params_t *p = (resize_params_t *)params;

// //     // Dispatch to the best hardware kernel detected at boot
// //     if (ctx->cpu_caps == ARCH_AVX512)
// //     {
// //         img_arch_avx512_resize(buf, p->target_w, p->target_h);
// //         // img_arch_avx2_resize(img_buffer_t);
// //     }
// //     else if (ctx->cpu_caps == ARCH_NEON)
// //     {
// //         img_arch_neon_resize(buf, p->target_w, p->target_h);
// //     }
// //     else
// //     {
// //         img_arch_scalar_resize(buf, p->target_w, p->target_h);
// //     }
// // }

// // // Batch Image Execution (SIMD-Interleaved)
// // void plugin_resize_batch(img_ctx_t *ctx, img_batch_t *batch, void *params)
// // {
// //     // Calls the batch-aware kernels from Phase 9
// //     img_arch_batch_resize(ctx, batch, (resize_params_t *)params);
// // }

// /* plugins/plugin_resize.c */
// #include "plugins/plugin_internal.h"
// #include "arch/arch_kernels.h"
// #include "memory/slab.h"

// void plugin_resize_single(img_ctx_t *ctx, img_buffer_t *buf, void *params)
// {
//     if (!params || !buf) return;
//     resize_params_t *p = (resize_params_t *)params;

//     // 1. Get a fresh block from the Slab for the resized image
//     uint8_t* out_mem = img_slab_alloc(ctx->pool);
//     img_buffer_t dst = img_buffer_create(out_mem, p->target_w, p->target_h, buf->channels);

//     // 2. Dispatch to silicon kernels using the (src, dst) contract
//     if (ctx->cpu_caps == ARCH_AVX512) {
//         img_arch_avx512_resize(buf, &dst);
//     } else if (ctx->cpu_caps == ARCH_AVX2) {
//         img_arch_avx2_resize(buf, &dst);
//     } else if (ctx->cpu_caps == ARCH_NEON) {
//         img_arch_neon_resize(buf, &dst);
//     } else {
//         img_arch_scalar_resize(buf, &dst);
//     }

//     // 3. IMPORTANT: Free old buffer memory and switch 'buf' to the new 'dst'
//     img_slab_free(ctx->pool, buf->data);
//     *buf = dst;
// }

/* plugins/plugin_resize.c */
#include "plugins/plugin_internal.h"
#include "arch/arch_kernels.h"
#include "memory/slab.h"
#include "core/image.h"

// Defined here to keep the Arch layer decoupled from Plugin logic
typedef struct
{
    uint32_t target_w;
    uint32_t target_h;
} resize_params_t;

/**
 * @brief Logic for single image resize.
 * Allocates new Slab memory, runs SIMD, then swaps pointers.
 */
void plugin_resize_single(img_ctx_t *ctx, img_buffer_t *buf, void *params)
{
    if (!params || !buf || !ctx)
        return;
    resize_params_t *p = (resize_params_t *)params;

    // 1. ALLOCATE: Get a fresh block for the result
    // Note: img_slab_alloc is O(1)
    uint8_t *out_mem = img_slab_alloc(ctx->pool);
    if (!out_mem)
        return;

    // 2. WRAP: Apply the 64-byte alignment rule for the new buffer
    img_buffer_t dst = img_buffer_create(out_mem, p->target_w, p->target_h, buf->channels);

    // 3. DISPATCH: Call the silicon-specific kernel
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

    // 4. SWAP: Release the old buffer and update the metadata
    img_slab_free(ctx->pool, buf->data);
    *buf = dst;
}

/**
 * @brief Logic for batch (8-way) resize.
 */
void plugin_resize_batch(img_ctx_t *ctx, img_batch_t *batch, void *params)
{
    if (!ctx || !batch || !params)
        return;
    img_arch_batch_resize(ctx, batch, params);
}
