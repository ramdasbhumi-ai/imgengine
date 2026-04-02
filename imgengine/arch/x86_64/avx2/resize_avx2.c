// /* arch/x86_64/avx2/resize_avx2.c */

#include <immintrin.h>
#include "arch/arch_interface.h"
#include "plugins/plugin_resize.h"

#include <immintrin.h>

void img_arch_avx2_resize(img_ctx_t *ctx, img_buffer_t *dst, void *params)
{
    (void)ctx;

    resize_params_t *p = (resize_params_t *)params;
    img_buffer_t *src = p->src;

    uint8_t *src_data = src->data;
    uint8_t *dst_data = dst->data;

    uint32_t scale_x = p->scale_x;
    uint32_t scale_y = p->scale_y;

    const uint32_t ch = src->channels;

    for (uint32_t dy = 0; dy < dst->height; dy++)
    {
        uint32_t sy = (dy * scale_y) >> 16;

        uint8_t *src_row = src_data + sy * src->stride;
        uint8_t *dst_row = dst_data + dy * dst->stride;

        uint32_t dx = 0;

        // 🔥 SIMD LOOP (process 32 bytes)
        for (; dx + 32 <= dst->width * ch; dx += 32)
        {
            uint32_t pixel_index = dx / ch;
            uint32_t sx = (pixel_index * scale_x) >> 16;

            __m256i pixels = _mm256_loadu_si256(
                (__m256i *)(src_row + sx * ch));

            _mm256_storeu_si256(
                (__m256i *)(dst_row + dx),
                pixels);
        }

        // 🔥 TAIL (remaining pixels)
        for (; dx < dst->width * ch; dx++)
        {
            uint32_t pixel_index = dx / ch;
            uint32_t sx = (pixel_index * scale_x) >> 16;

            dst_row[dx] = src_row[sx * ch + (dx % ch)];
        }
    }
}

// static inline __m256i lerp_epi8(__m256i a, __m256i b, __m256i w)
// {
//     // (a + (w * (b - a)) >> 8)
//     __m256i diff = _mm256_sub_epi8(b, a);
//     __m256i mul = _mm256_mullo_epi16(diff, w);
//     __m256i res = _mm256_add_epi8(a, _mm256_srli_epi16(mul, 8));
//     return res;
// }

// void img_arch_resize_avx2(img_ctx_t *ctx, img_buffer_t *dst, void *params)
// {
//     resize_params_t *p = (resize_params_t *)params;
//     img_buffer_t *src = p->src;

//     (void)ctx;

//     const int src_w = src->width;
//     const int src_h = src->height;
//     const int dst_w = p->target_w;
//     const int dst_h = p->target_h;
//     const int ch = src->channels;

//     float scale_x = (float)src_w / dst_w;
//     float scale_y = (float)src_h / dst_h;

//     for (int y = 0; y < dst_h; y++)
//     {
//         float fy = y * scale_y;
//         int y0 = (int)fy;
//         int y1 = (y0 + 1 < src_h) ? y0 + 1 : y0;
//         float wy = fy - y0;

//         uint8_t *row0 = src->data + y0 * src->stride;
//         uint8_t *row1 = src->data + y1 * src->stride;
//         uint8_t *drow = dst->data + y * dst->stride;

//         for (int x = 0; x < dst_w; x += 32 / ch) // process vector width
//         {
//             float fx = x * scale_x;
//             int x0 = (int)fx;
//             int x1 = (x0 + 1 < src_w) ? x0 + 1 : x0;
//             float wx = fx - x0;

//             uint8_t *p00 = row0 + x0 * ch;
//             uint8_t *p01 = row0 + x1 * ch;
//             uint8_t *p10 = row1 + x0 * ch;
//             uint8_t *p11 = row1 + x1 * ch;

//             // Load 256-bit (32 bytes)
//             __m256i v00 = _mm256_loadu_si256((__m256i *)p00);
//             __m256i v01 = _mm256_loadu_si256((__m256i *)p01);
//             __m256i v10 = _mm256_loadu_si256((__m256i *)p10);
//             __m256i v11 = _mm256_loadu_si256((__m256i *)p11);

//             // Convert weights to vector
//             uint16_t wx_i = (uint16_t)(wx * 256);
//             uint16_t wy_i = (uint16_t)(wy * 256);

//             __m256i wx_vec = _mm256_set1_epi16(wx_i);
//             __m256i wy_vec = _mm256_set1_epi16(wy_i);

//             // Horizontal lerp
//             __m256i top = lerp_epi8(v00, v01, wx_vec);
//             __m256i bot = lerp_epi8(v10, v11, wx_vec);

//             // Vertical lerp
//             __m256i out = lerp_epi8(top, bot, wy_vec);

//             _mm256_storeu_si256((__m256i *)(drow + x * ch), out);
//         }
//     }
// }

// #include <immintrin.h>
// #include "plugins/plugin_resize.h"

// void img_arch_avx2_resize(img_ctx_t *ctx, img_buffer_t *dst, void *params)
// {
//     resize_params_t *p = (resize_params_t *)params;
//     img_buffer_t *src = p->src;

//     uint8_t *src_data = src->data;
//     uint8_t *dst_data = dst->data;

//     uint32_t scale_x = p->scale_x;
//     uint32_t scale_y = p->scale_y;

//     for (uint32_t dy = 0; dy < dst->height; dy++)
//     {
//         // 🔥 FIXED-POINT → integer pixel
//         uint32_t sy = (dy * scale_y) >> 16;

//         uint8_t *src_row = src_data + sy * src->stride;
//         uint8_t *dst_row = dst_data + dy * dst->stride;

//         for (uint32_t dx = 0; dx < dst->width; dx += 32)
//         {
//             // Compute source X positions (scalar for now)
//             uint32_t sx = (dx * scale_x) >> 16;

//             // Load 32 bytes
//             __m256i pixels = _mm256_loadu_si256((__m256i *)(src_row + sx * src->channels));

//             // Store
//             _mm256_storeu_si256((__m256i *)(dst_row + dx * dst->channels), pixels);
//         }
//     }
// }