/* arch/arch_kernels.h */
#ifndef IMGENGINE_ARCH_KERNELS_H
#define IMGENGINE_ARCH_KERNELS_H

#include "core/image.h"

// Scalar Fallbacks (Mandatory)
void img_arch_scalar_resize(img_buffer_t *src, img_buffer_t *dst);
void img_arch_scalar_grayscale(img_buffer_t *buf);

// AVX2 Optimisations
void img_arch_avx2_resize(img_buffer_t *src, img_buffer_t *dst);
void img_arch_avx2_grayscale(img_buffer_t *buf);

// AVX-512 (Google-Grade Server Path)
#ifdef __AVX512F__
void img_arch_avx512_resize(img_buffer_t *src, img_buffer_t *dst);
#endif

#endif
