// src/pipeline/crop_marks.c

// Draw ISO crop marks at each photo cell corner.
// Ported from crop_plugin.c and crop_marks.c, renamed.

#include "core/buffer.h"
#include "pipeline/layout.h"
#include "api/v1/img_error.h"

/* Draw black pixels — inline, no overhead */
static inline void set_pixel(
    img_buffer_t *canvas, int32_t x, int32_t y)
{
    if (x < 0 || y < 0 ||
        (uint32_t)x >= canvas->width ||
        (uint32_t)y >= canvas->height)
        return;

    uint8_t *p = canvas->data +
                 (size_t)y * canvas->stride +
                 (size_t)x * 3;
    p[0] = p[1] = p[2] = 0;
}

static void draw_h(img_buffer_t *c, int32_t x, int32_t y,
                   int32_t len, uint32_t th)
{
    for (uint32_t t = 0; t < th; t++)
        for (int32_t i = 0; i < len; i++)
            set_pixel(c, x + i, y + (int32_t)t);
}

static void draw_v(img_buffer_t *c, int32_t x, int32_t y,
                   int32_t len, uint32_t th)
{
    for (uint32_t t = 0; t < th; t++)
        for (int32_t i = 0; i < len; i++)
            set_pixel(c, x + (int32_t)t, y + i);
}

/*
 * img_draw_crop_marks()
 *
 * Draw ISO-standard crop marks at each photo cell corner.
 * Marks are aligned to the photo edge (not the bleed edge)
 * so the lab knows exactly where to cut.
 */
img_result_t img_draw_crop_marks(
    img_buffer_t *canvas,
    const img_layout_t *layout,
    const img_job_t *job)
{
    if (!canvas || !layout || job->crop_mark_px == 0)
        return IMG_SUCCESS;

    int32_t d = (int32_t)job->crop_offset_px;
    int32_t len = (int32_t)job->crop_mark_px;
    uint32_t th = job->crop_thickness;

    for (uint32_t i = 0; i < layout->count; i++)
    {
        const img_cell_t *c = &layout->cells[i];
        int32_t x = (int32_t)c->x;
        int32_t y = (int32_t)c->y;
        int32_t x2 = x + (int32_t)c->w;
        int32_t y2 = y + (int32_t)c->h;

        /* TOP LEFT */
        draw_h(canvas, x - d - len, y - d, len, th);
        draw_v(canvas, x - d, y - d - len, len, th);

        /* TOP RIGHT */
        draw_h(canvas, x2 + d, y - d, len, th);
        draw_v(canvas, x2 + d, y - d - len, len, th);

        /* BOTTOM LEFT */
        draw_h(canvas, x - d - len, y2 + d, len, th);
        draw_v(canvas, x - d, y2 + d, len, th);

        /* BOTTOM RIGHT */
        draw_h(canvas, x2 + d, y2 + d, len, th);
        draw_v(canvas, x2 + d, y2 + d, len, th);
    }

    return IMG_SUCCESS;
}
