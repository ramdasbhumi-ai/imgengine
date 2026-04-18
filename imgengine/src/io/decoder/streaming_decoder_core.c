// ./src/io/decoder/streaming_decoder_core.c
#define _GNU_SOURCE

#include "io/streaming_decoder_internal.h"
#include "io/io_vfs.h"

#include "core/buffer.h"
#include "memory/slab.h"
#include "security/input_validator.h"
#include "security/bounds_check.h"

#include <turbojpeg.h>

#define HEADER_SIZE 1024

int img_decode_stream_core(
    img_slab_pool_t *pool,
    img_stream_t *stream,
    img_buffer_t *out)
{
    if (!pool || !stream || !out)
        return -1;

    uint8_t header[HEADER_SIZE];
    size_t read_bytes = 0;

    if (img_vfs_read(stream, header, HEADER_SIZE, &read_bytes) != IMG_SUCCESS)
        return -1;

    tjhandle tj = tjInitDecompress();
    if (!tj)
        return -1;

    int w = 0, h = 0, subsamp = 0, cs = 0;

    if (tjDecompressHeader3(tj, header, read_bytes, &w, &h, &subsamp, &cs) != 0)
    {
        tjDestroy(tj);
        return -1;
    }

    if (img_security_validate_request(w, h, stream->size) != IMG_SUCCESS)
    {
        tjDestroy(tj);
        return -1;
    }

    size_t stride = (size_t)w * 3;
    size_t required = stride * (size_t)h;

    size_t block = img_slab_block_size(pool);
    if (required > block)
    {
        tjDestroy(tj);
        return -1;
    }

    uint8_t *mem = img_slab_alloc(pool);
    if (!mem)
    {
        tjDestroy(tj);
        return -1;
    }

    uint8_t *tmp = img_slab_alloc(pool);
    if (!tmp)
    {
        img_slab_free(pool, mem);
        tjDestroy(tj);
        return -1;
    }

    if (img_vfs_read(stream, tmp, required, &read_bytes) != IMG_SUCCESS)
    {
        img_slab_free(pool, tmp);
        img_slab_free(pool, mem);
        tjDestroy(tj);
        return -1;
    }

    if (!img_bounds_check(tmp, read_bytes, stream->data, stream->size))
    {
        img_slab_free(pool, tmp);
        img_slab_free(pool, mem);
        tjDestroy(tj);
        return -1;
    }

    if (tjDecompress2(
            tj,
            tmp,
            read_bytes,
            mem,
            w,
            stride,
            h,
            TJPF_RGB,
            TJFLAG_FASTDCT | TJFLAG_FASTUPSAMPLE) != 0)
    {
        img_slab_free(pool, tmp);
        img_slab_free(pool, mem);
        tjDestroy(tj);
        return -1;
    }

    out->data = mem;
    out->width = (uint32_t)w;
    out->height = (uint32_t)h;
    out->channels = 3;
    out->stride = (uint32_t)stride;

    img_slab_free(pool, tmp);
    tjDestroy(tj);
    return 0;
}
