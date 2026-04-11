// src/pipeline/bleed.c

// Bleed: extend edge pixels outward past photo boundary.
// Ported from bleed_plugin.c, fully renamed, no old types.

#include "core/buffer.h"
#include "pipeline/layout.h"
#include "api/v1/img_error.h"
#include <string.h>

/*
 * img_apply_bleed()
 *
 * For each photo cell, extend the edge pixels outward by bleed_px.
 * This prevents white borders when the print sheet is cut.
 *
 * Operates in-place on the canvas buffer.
 * No allocation. Pure pixel copy.
 */
img_result_t img_apply_bleed(
    img_buffer_t *canvas,
    const img_layout_t *layout,
    uint32_t bleed_px)
{
    if (!canvas || !layout || bleed_px == 0)
        return IMG_SUCCESS;

    const uint32_t ch = 3;
    const uint32_t cw = canvas->width;
    const uint32_t ch_px = canvas->height;

    for (uint32_t i = 0; i < layout->count; i++)
    {
        const img_cell_t *c = &layout->cells[i];
        const uint32_t x = c->x, y = c->y;
        const uint32_t w = c->w, h = c->h;
        const uint32_t row_bytes = w * ch;

        /* TOP — copy first row upward */
        for (uint32_t b = 1; b <= bleed_px; b++)
        {
            if (y < b)
                break;
            memcpy(canvas->data + (size_t)(y - b) * canvas->stride + x * ch,
                   canvas->data + (size_t)y * canvas->stride + x * ch,
                   row_bytes);
        }

        /* BOTTOM — copy last row downward */
        for (uint32_t b = 1; b <= bleed_px; b++)
        {
            uint32_t ty = y + h - 1 + b;
            if (ty >= ch_px)
                break;
            memcpy(canvas->data + (size_t)ty * canvas->stride + x * ch,
                   canvas->data + (size_t)(y + h - 1) * canvas->stride + x * ch,
                   row_bytes);
        }

        /* LEFT + RIGHT — row by row */
        uint32_t y_start = (y >= bleed_px) ? y - bleed_px : 0;
        uint32_t y_end = (y + h + bleed_px < ch_px) ? y + h + bleed_px : ch_px;

        for (uint32_t py = y_start; py < y_end; py++)
        {
            uint8_t *row = canvas->data + (size_t)py * canvas->stride;

            /* LEFT */
            const uint8_t *src_l = row + x * ch;
            for (uint32_t b = 1; b <= bleed_px; b++)
            {
                if (x < b)
                    break;
                uint8_t *dst = row + (x - b) * ch;
                dst[0] = src_l[0];
                dst[1] = src_l[1];
                dst[2] = src_l[2];
            }

            /* RIGHT */
            const uint8_t *src_r = row + (x + w - 1) * ch;
            for (uint32_t b = 1; b <= bleed_px; b++)
            {
                uint32_t tx = x + w - 1 + b;
                if (tx >= cw)
                    break;
                uint8_t *dst = row + tx * ch;
                dst[0] = src_r[0];
                dst[1] = src_r[1];
                dst[2] = src_r[2];
            }
        }
    }

    return IMG_SUCCESS;
}
