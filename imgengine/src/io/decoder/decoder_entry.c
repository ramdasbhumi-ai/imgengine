// io/decoder/decoder_entry.c

#include "io/decoder/decoder_entry.h"
#include <turbojpeg.h>
#include "api/v1/img_buffer_utils.h"
#include "memory/slab.h"

int img_decode_to_buffer(
    img_ctx_t *ctx,
    uint8_t *input,
    size_t size,
    img_buffer_t *out)
{
    if (!ctx || !input || !out)
        return -1;

    tjhandle tj = tjInitDecompress();
    if (!tj)
        return -1;

    int w, h, subsamp, cs;

    if (tjDecompressHeader3(
            tj, input, size, &w, &h, &subsamp, &cs) != 0)
    {
        tjDestroy(tj);
        return -1;
    }

    const int ch = 3;
    const size_t stride = w * ch;
    const size_t required = stride * h;

    size_t block = img_slab_block_size(ctx->local_pool);

    // 🔥 HARD SAFETY
    if (required > block)
    {
        tjDestroy(tj);
        return -1;
    }

    uint8_t *mem = img_slab_alloc(ctx->local_pool);
    if (!mem)
    {
        tjDestroy(tj);
        return -1;
    }

    if (tjDecompress2(
            tj,
            input,
            size,
            mem,
            w,
            stride,
            h,
            TJPF_RGB,
            TJFLAG_FASTDCT | TJFLAG_FASTUPSAMPLE) != 0)
    {
        img_slab_free(ctx->local_pool, mem);
        tjDestroy(tj);
        return -1;
    }

    *out = img_buffer_create(mem, w, h, ch);

    tjDestroy(tj);
    return 0;
}