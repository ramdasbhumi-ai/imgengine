// pipeline/pipeline_fused.c

#include "pipeline/pipeline_fused.h"
#include "pipeline/pipeline_types.h"
#include "core/opcodes.h"
#include "core/buffer.h"

#if defined(__AVX2__)
#include <immintrin.h>
#endif

/*
 * 🔥 STATIC STORAGE (NO MALLOC EVER)
 */
static img_pipeline_fused_t g_fused;

/*
 * 🔥 FORWARD DECLARATIONS
 */
static void fused_exec_scalar(img_ctx_t *ctx, img_buffer_t *buf);

#if defined(__AVX2__)
static void fused_exec_avx2(img_ctx_t *ctx, img_buffer_t *buf);
#endif

/*
 * 🔥 FUSION COMPILER (PIPELINE → IR)
 */
int img_pipeline_fuse(
    const img_pipeline_desc_t *in,
    img_pipeline_fused_t *out)
{
    if (!in || !out)
        return -1;

    if (in->count == 0 || in->count > IMG_MAX_FUSED_OPS)
        return -1;

    g_fused.count = 0;

    for (uint32_t i = 0; i < in->count; i++)
    {
        uint32_t op = in->ops[i].op_code;

        switch (op)
        {
        case OP_GRAYSCALE:
            g_fused.ops[g_fused.count++] = FUSED_OP_GRAYSCALE;
            break;

        case OP_RESIZE:
            /* not fusible yet */
            break;

        default:
            return -1;
        }
    }

#if defined(__AVX2__)
    g_fused.fn = fused_exec_avx2;
#else
    g_fused.fn = fused_exec_scalar;
#endif

    *out = g_fused;
    return 0;
}
#if defined(__AVX2__)
static void fused_exec_avx2(
    img_ctx_t *ctx,
    img_buffer_t *buf)
{
    (void)ctx;

    uint32_t w = buf->width;
    uint32_t h = buf->height;
    uint32_t ch = buf->channels;

    uint8_t *data = buf->data;

    const __m256i zero = _mm256_setzero_si256();
    const __m256i coeff = _mm256_set1_epi16(77);
    const __m256i bright = _mm256_set1_epi16(20);

    for (uint32_t y = 0; y < h; y++)
    {
        uint8_t *row = data + y * buf->stride;

        __builtin_prefetch(row + 64, 0, 1);

        uint32_t x = 0;
        uint32_t len = w * ch;

        for (; x + 32 <= len; x += 32)
        {
            __m256i v = _mm256_loadu_si256((__m256i *)(row + x));

            __m256i lo = _mm256_unpacklo_epi8(v, zero);
            __m256i hi = _mm256_unpackhi_epi8(v, zero);

            for (uint32_t op = 0; op < g_fused.count; op++)
            {
                switch (g_fused.ops[op])
                {
                case FUSED_OP_GRAYSCALE:
                    lo = _mm256_srli_epi16(_mm256_mullo_epi16(lo, coeff), 8);
                    hi = _mm256_srli_epi16(_mm256_mullo_epi16(hi, coeff), 8);
                    break;

                case FUSED_OP_BRIGHTNESS:
                    lo = _mm256_adds_epu16(lo, bright);
                    hi = _mm256_adds_epu16(hi, bright);
                    break;
                }
            }

            __m256i out = _mm256_packus_epi16(lo, hi);

            _mm256_storeu_si256((__m256i *)(row + x), out);
        }

        for (; x < len; x++)
        {
            uint8_t px = row[x];

            for (uint32_t op = 0; op < g_fused.count; op++)
            {
                if (g_fused.ops[op] == FUSED_OP_GRAYSCALE)
                    px = (px * 77) >> 8;

                else if (g_fused.ops[op] == FUSED_OP_BRIGHTNESS)
                {
                    uint16_t tmp = px + 20;
                    px = (tmp > 255) ? 255 : tmp;
                }
            }

            row[x] = px;
        }
    }
}
#endif

static void fused_exec_scalar(
    img_ctx_t *ctx,
    img_buffer_t *buf)
{
    (void)ctx;

    uint32_t w = buf->width;
    uint32_t h = buf->height;
    uint32_t ch = buf->channels;

    uint8_t *data = buf->data;

    for (uint32_t y = 0; y < h; y++)
    {
        uint8_t *row = data + y * buf->stride;

        for (uint32_t x = 0; x < w * ch; x++)
        {
            uint8_t px = row[x];

            for (uint32_t op = 0; op < g_fused.count; op++)
            {
                if (g_fused.ops[op] == FUSED_OP_GRAYSCALE)
                    px = (px * 77) >> 8;

                else if (g_fused.ops[op] == FUSED_OP_BRIGHTNESS)
                {
                    uint16_t tmp = px + 20;
                    px = (tmp > 255) ? 255 : tmp;
                }
            }

            row[x] = px;
        }
    }
}

void img_pipeline_execute_fused(
    img_ctx_t *ctx,
    img_pipeline_fused_t *pipe,
    img_buffer_t *buf)
{
    if (!pipe || !pipe->fn)
        return;

    pipe->fn(ctx, buf);
}

void img_pipeline_execute_fused_batch(
    img_ctx_t *ctx,
    img_pipeline_fused_t *pipe,
    img_batch_t *batch)
{
    if (!pipe || !batch)
        return;

    for (uint32_t i = 0; i < batch->count; i++)
    {
        pipe->fn(ctx, &batch->buffers[i]);
    }
}