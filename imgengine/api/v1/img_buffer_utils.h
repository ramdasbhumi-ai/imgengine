// api/v1/img_buffer_utils.h

#ifndef IMGENGINE_IMG_BUFFER_UTILS_H
#define IMGENGINE_IMG_BUFFER_UTILS_H

#include "core/buffer.h"
#include "memory/slab.h"

/*
 * 🔥 Create buffer INSIDE slab (header + data contiguous)
 */
static inline img_buffer_t img_buffer_from_slab(
    img_slab_pool_t *pool,
    uint32_t w,
    uint32_t h,
    uint32_t ch)
{
    img_buffer_t buf = {0};

    size_t stride = w * ch;
    size_t data_size = stride * h;

    size_t total = sizeof(img_buf_header_t) + data_size;

    uint8_t *mem = img_slab_alloc(pool);
    if (!mem)
        return buf;

    img_buf_header_t *hdr = (img_buf_header_t *)mem;
    hdr->ref = 1;
    hdr->flags = 0;

    buf.data = mem + sizeof(img_buf_header_t);
    buf.width = w;
    buf.height = h;
    buf.channels = ch;
    buf.stride = stride;

    return buf;
}

#endif

// #ifndef IMGENGINE_IMG_BUFFER_UTILS_H
// #define IMGENGINE_IMG_BUFFER_UTILS_H
// #include "core/buffer.h"

// // #include "api/v1/img_types.h"

// // 🔥 zero-cost helper
// static inline img_buffer_t img_buffer_create(
//     uint8_t *data,
//     uint32_t w,
//     uint32_t h,
//     uint32_t ch)
// {
//     img_buffer_t buf;
//     buf.data = data;
//     buf.width = w;
//     buf.height = h;
//     buf.channels = ch;
//     buf.stride = w * ch;
//     return buf;
// }

// #endif
