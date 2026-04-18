// ./src/io/decoder/streaming_decoder.c
#include "io/streaming_decoder.h"
#include "io/streaming_decoder_internal.h"
#include "core/context_internal.h"

int img_decode_stream(
    img_ctx_t *ctx,
    img_stream_t *stream,
    img_buffer_t *out)
{
    if (!ctx || !stream || !out)
        return -1;

    return img_decode_stream_core(ctx->local_pool, stream, out);
}
