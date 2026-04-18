// ./include/pipeline/layout_resize_internal.h
#ifndef IMGENGINE_PIPELINE_LAYOUT_RESIZE_INTERNAL_H
#define IMGENGINE_PIPELINE_LAYOUT_RESIZE_INTERNAL_H

#include "pipeline/layout_resize.h"

img_result_t img_layout_resize_validate_alloc(
    const img_buffer_t *src,
    img_buffer_t *dst,
    uint32_t new_w,
    uint32_t new_h,
    img_slab_pool_t *pool,
    uint8_t **mem_out,
    uint32_t *stride_out);

void img_layout_resize_resample_bilinear(
    const img_buffer_t *src,
    uint8_t *mem,
    uint32_t new_w,
    uint32_t new_h,
    uint32_t stride);

img_result_t img_layout_prepare_fill_commit(
    const img_buffer_t *tmp,
    img_buffer_t *dst,
    img_slab_pool_t *pool,
    uint32_t cell_w,
    uint32_t cell_h);

img_result_t img_layout_prepare_fit_commit(
    const img_buffer_t *tmp,
    img_buffer_t *dst,
    img_slab_pool_t *pool,
    uint32_t cell_w,
    uint32_t cell_h,
    uint8_t bg_r,
    uint8_t bg_g,
    uint8_t bg_b);

#endif /* IMGENGINE_PIPELINE_LAYOUT_RESIZE_INTERNAL_H */
