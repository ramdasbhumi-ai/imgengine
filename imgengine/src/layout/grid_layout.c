// // src/layout/grid_layout.c

#include "imgengine/image.h"
#include "imgengine/api.h"
#include "imgengine/resize.h"
#include "imgengine/context.h"
#include <stdio.h>

extern void img_blit_avx2(img_t *, const img_t *, int, int);
extern void draw_crop_marks(img_t *, int, int, int, int, int, int);

// 🔥 EDGE EXTENSION BLEED (REAL BLEED)
static void apply_bleed(img_t *dst, const img_t *src,
                        int x, int y, int bleed)
{
    int pw = src->width;
    int ph = src->height;

    // Top & Bottom bleed
    for (int i = 0; i < bleed; i++)
    {
        for (int px = 0; px < pw; px++)
        {
            int src_top = (0 * pw + px) * 3;
            int src_bot = ((ph - 1) * pw + px) * 3;

            int dst_top = ((y - i - 1) * dst->width + (x + px)) * 3;
            int dst_bot = ((y + ph + i) * dst->width + (x + px)) * 3;

            for (int c = 0; c < 3; c++)
            {
                dst->data[dst_top + c] = src->data[src_top + c];
                dst->data[dst_bot + c] = src->data[src_bot + c];
            }
        }
    }

    // Left & Right bleed
    for (int i = 0; i < bleed; i++)
    {
        for (int py = 0; py < ph; py++)
        {
            int src_l = (py * pw + 0) * 3;
            int src_r = (py * pw + (pw - 1)) * 3;

            int dst_l = ((y + py) * dst->width + (x - i - 1)) * 3;
            int dst_r = ((y + py) * dst->width + (x + pw + i)) * 3;

            for (int c = 0; c < 3; c++)
            {
                dst->data[dst_l + c] = src->data[src_l + c];
                dst->data[dst_r + c] = src->data[src_r + c];
            }
        }
    }
}

int layout_grid(img_t *canvas,
                const img_t *photo,
                const img_job_t *job,
                img_ctx_t *ctx)
{
    int cols = job->cols;
    int rows = job->rows;
    int gap = job->gap;
    int padding = job->padding;

    int pw = photo->width;
    int ph = photo->height;

    // =========================
    // FIT-TO-PAGE SCALING
    // =========================
    int usable_w = canvas->width - 2 * padding;
    int usable_h = canvas->height - 2 * padding;

    float scale_x = (float)usable_w /
                    (cols * pw + (cols - 1) * gap);

    float scale_y = (float)usable_h /
                    (rows * ph + (rows - 1) * gap);

    float scale = scale_x < scale_y ? scale_x : scale_y;
    if (scale > 1.0f)
        scale = 1.0f;

    int final_pw = (int)(pw * scale);
    int final_ph = (int)(ph * scale);

    // =========================
    // PRE-RESIZE
    // =========================
    img_t scaled;
    if (!img_resize(photo, &scaled, &ctx->pool, final_pw, final_ph))
        return 0;

    // =========================
    // HYBRID ALIGNMENT
    // =========================
    int total_w = cols * final_pw + (cols - 1) * gap;

    int start_x = (canvas->width - total_w) / 2;
    int start_y = padding;

    if (start_x < padding)
        start_x = padding;

    // =========================
    // GRID LOOP
    // =========================
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int x = start_x + c * (final_pw + gap);
            int y = start_y + r * (final_ph + gap);

            // ✅ BLIT
            img_blit_avx2(canvas, &scaled, x, y);

            // ✅ REAL BLEED
            if (job->bleed_px > 0)
                apply_bleed(canvas, &scaled, x, y, job->bleed_px);

            // ✅ CROP MARKS (DYNAMIC)
            draw_crop_marks(canvas,
                            x, y,
                            final_pw, final_ph,
                            job->crop_mark_px,
                            job->crop_thickness);
        }
    }

    return 1;
}

// #include "imgengine/image.h"
// #include <stdio.h>

// // external SIMD + crop mark
// extern void img_blit_avx2(img_t *, const img_t *, int, int);
// extern void draw_crop_marks(img_t *, int, int, int, int, int, int);

// int layout_grid(img_t *canvas,
//                 const img_t *photo,
//                 int cols, int rows,
//                 int gap, int padding)
// {
//     int pw = photo->width;
//     int ph = photo->height;

//     // =========================
//     // PRO HYBRID POSITION
//     // =========================
//     int total_w = cols * pw + (cols - 1) * gap;
//     int start_x = (canvas->width - total_w) / 2;
//     int start_y = padding;

//     if (start_x < padding)
//         start_x = padding;

//     // =========================
//     // GRID LOOP
//     // =========================
//     for (int r = 0; r < rows; r++)
//     {
//         for (int c = 0; c < cols; c++)
//         {
//             int x = start_x + c * (pw + gap);
//             int y = start_y + r * (ph + gap);

//             if (x + pw > canvas->width || y + ph > canvas->height)
//             {
//                 printf("❌ Overflow at r=%d c=%d\n", r, c);
//                 return 0;
//             }

//             // ✅ FAST BLIT (SIMD)
//             img_blit_avx2(canvas, photo, x, y);

//             // ✅ CROP MARKS (fixed values for now)
//             draw_crop_marks(canvas,
//                             x, y,
//                             pw, ph,
//                             20, // mark length
//                             2); // thickness
//         }
//     }

//     return 1;
// }