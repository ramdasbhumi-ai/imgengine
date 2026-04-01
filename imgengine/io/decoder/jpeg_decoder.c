/* io/decoder/jpeg_decoder.c */
// #include <turbojpeg.h>
// #include "core/context.h"

/* io/decoder/jpeg_decoder.c */
#include <turbojpeg.h>
#include "api/v1/img_error.h" // For img_result_t
#include "io/io_vfs.h"        // For img_stream_t
#include "core/image.h"       // For img_buffer_t
#include "core/context.h"     // For img_ctx_t

img_result_t img_decode_jpeg_hardened(img_ctx_t *ctx, img_stream_t *stream, img_buffer_t *out)
{
    tjhandle handle = tjInitDecompress();
    int w, h, subsamp, cp;

    // 1. HEADER ONLY (Low latency)
    tjDecompressHeader3(handle, stream->data, stream->size, &w, &h, &subsamp, &cp);

    // 2. SECURITY: Reject if image is impossibly large for our Slabs
    if (w * h * 4 > 64 * 1024 * 1024)
    { // 64MB Limit
        tjDestroy(handle);
        return IMG_ERR_NOMEM;
    }

    // 3. ZERO-COPY DECODE: Directly into Slab
    uint8_t *slab = img_slab_alloc(ctx->pool);
    *out = img_buffer_create(slab, w, h, 3);

    tjDecompress2(handle, stream->data, stream->size, out->data, w, out->stride, h, TJPF_RGB, TJFLAG_FASTDCT);

    tjDestroy(handle);
    return IMG_SUCCESS;
}
