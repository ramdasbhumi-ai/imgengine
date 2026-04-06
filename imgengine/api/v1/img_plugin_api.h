// api/v1/img_plugin_abi.h

#ifndef IMGENGINE_PLUGIN_ABI_H
#define IMGENGINE_PLUGIN_ABI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define IMG_PLUGIN_ABI_VERSION 1

#define IMG_CAP_SINGLE (1 << 0)
#define IMG_CAP_BATCH (1 << 1)
#define IMG_CAP_SIMD (1 << 2)
#define IMG_CAP_ZERO_COPY (1 << 3)

    // Opaque types
    typedef struct img_ctx img_ctx_t;
    typedef struct img_buffer img_buffer_t;
    typedef struct img_batch img_batch_t;

    // Function types
    typedef void (*img_op_fn)(img_ctx_t *, img_buffer_t *, void *);
    typedef void (*img_batch_op_fn)(img_ctx_t *, img_batch_t *, void *);

    // ABI descriptor
    typedef struct
    {
        uint32_t abi_version;
        uint32_t op_code;
        const char *name;
        uint32_t capabilities;

        img_op_fn single_exec;
        img_batch_op_fn batch_exec;

    } img_plugin_descriptor_t;

// Symbol name
#define IMG_PLUGIN_EXPORT_SYMBOL "img_plugin_get_descriptor"

    typedef const img_plugin_descriptor_t *(*img_plugin_getter_fn)(void);

#ifdef __cplusplus
}
#endif

#endif