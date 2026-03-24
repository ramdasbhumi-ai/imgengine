// src/ops/resize.c

#include "imgengine/image.h"

int img_resize(const img_t *src, img_t *dst,
               mem_pool_t *mp,
               int new_w, int new_h)
{

    if (!img_create(dst, mp, new_w, new_h, 3))
        return 0;

    int sw = src->width, sh = src->height;

    for (int y = 0; y < new_h; y++)
    {
        for (int x = 0; x < new_w; x++)
        {

            float gx = (float)x / new_w * sw;
            float gy = (float)y / new_h * sh;

            int gxi = (int)gx;
            int gyi = (int)gy;

            for (int c = 0; c < 3; c++)
            {
                dst->data[(y * new_w + x) * 3 + c] =
                    src->data[(gyi * sw + gxi) * 3 + c];
            }
        }
    }

    return 1;
}