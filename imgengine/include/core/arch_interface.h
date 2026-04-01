/* include/arch/arch_interface.h */
#ifndef IMGENGINE_ARCH_INTERFACE_H
#define IMGENGINE_ARCH_INTERFACE_H

#include "core/image.h"

// Function pointer signature for all image operations
typedef void (*img_kernel_fn)(img_ctx_t *ctx, img_buffer_t *buf, void *params);

// Hardware-Specific Kernel Prototypes
// x86_64
void img_kernel_resize_avx512(img_ctx_t *ctx, img_buffer_t *buf, void *params);
void img_kernel_resize_avx2(img_ctx_t *ctx, img_buffer_t *buf, void *params);

// aarch64
void img_kernel_resize_neon(img_ctx_t *ctx, img_buffer_t *buf, void *params);

#endif
