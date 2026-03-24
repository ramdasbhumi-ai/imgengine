// src/layout/grid_layout.c

#include "imgengine/image.h"

extern void draw_crop_marks(img_t *, int, int, int, int, int, int);

int layout_grid(img_t *canvas,
                const img_t *photo,
                int cols, int rows, int gap, int padding)
{
    int pw = photo->width;
    int ph = photo->height;

    int x = padding;
    int y = padding;

    for (int r = 0; r < rows; r++)
    {
        x = padding;

        for (int c = 0; c < cols; c++)
        {
            if (x + pw > canvas->width || y + ph > canvas->height)
                return 0;

            img_blit_avx2(canvas, &scaled, x, y);

            // 🔥 ADD THIS
            draw_crop_marks(canvas,
                            x, y,
                            final_pw, final_ph,
                            job->crop_mark_px,
                            job->crop_thickness);

            for (int iy = 0; iy < ph; iy++)
            {
                for (int ix = 0; ix < pw; ix++)
                {
                    int src_idx = (iy * pw + ix) * 3;
                    int dst_idx = ((y + iy) * canvas->width + (x + ix)) * 3;

                    canvas->data[dst_idx + 0] = photo->data[src_idx + 0];
                    canvas->data[dst_idx + 1] = photo->data[src_idx + 1];
                    canvas->data[dst_idx + 2] = photo->data[src_idx + 2];
                }
            }

            x += pw + gap;
        }

        y += ph + gap;
    }

    return 1;
}
