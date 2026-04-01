// /* arch/arch_kernels.h */
// #ifndef IMGENGINE_ARCH_KERNELS_H
// #define IMGENGINE_ARCH_KERNELS_H

// #include "core/image.h"
// #include "hot/batch.h"

// // All resize kernels must follow the (src, dst) contract
// void img_arch_scalar_resize(img_buffer_t *src, img_buffer_t *dst);
// void img_arch_avx2_resize(img_buffer_t *src, img_buffer_t *dst);
// void img_arch_avx512_resize(img_buffer_t *src, img_buffer_t *dst);
// void img_arch_neon_resize(img_buffer_t *src, img_buffer_t *dst);

// // Scalar Fallbacks (Mandatory)
// void img_arch_scalar_resize(img_buffer_t *src, img_buffer_t *dst);
// void img_arch_scalar_grayscale(img_buffer_t *buf);

// // AVX2 Optimisations
// void img_arch_avx2_resize(img_buffer_t *src, img_buffer_t *dst);
// void img_arch_avx2_grayscale(img_buffer_t *buf);

// // // AVX-512 (Google-Grade Server Path)
// #ifdef __AVX512F__
// void img_arch_avx512_resize(img_buffer_t *src, img_buffer_t *dst);

// // Batch Kernels
// void img_arch_batch_resize(img_ctx_t *ctx, img_batch_t *batch, void *params);

// #endif

/* arch/arch_kernels.h */
#ifndef IMGENGINE_ARCH_KERNELS_H
#define IMGENGINE_ARCH_KERNELS_H

#include "core/image.h"
#include "hot/batch.h"
#include "core/context.h"

// --- Resize Kernels (Source -> Destination Contract) ---
void img_arch_scalar_resize(img_buffer_t *src, img_buffer_t *dst);
void img_arch_avx2_resize(img_buffer_t *src, img_buffer_t *dst);
void img_arch_avx512_resize(img_buffer_t *src, img_buffer_t *dst);
void img_arch_neon_resize(img_buffer_t *src, img_buffer_t *dst);

// --- Color Kernels (In-Place) ---
void img_arch_scalar_grayscale(img_buffer_t *buf);
void img_arch_avx2_grayscale(img_buffer_t *buf);

// --- Batch Kernels (8-way Vectorization) ---
void img_arch_batch_resize(img_ctx_t *ctx, img_batch_t *batch, void *params);

#endif
