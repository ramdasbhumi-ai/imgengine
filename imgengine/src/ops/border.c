// src/ops/border

#include "imgengine/image.h"
#include <string.h>

int img_add_border(const img_t *src, img_t *dst,
                   mem_pool_t *mp,
                   int border)
{

    int nw = src->width + 2 * border;
    int nh = src->height + 2 * border;

    if (!img_create(dst, mp, nw, nh, 3))
        return 0;

    memset(dst->data, 0, nw * nh * 3);

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {

            int src_i = (y * src->width + x) * 3;
            int dst_i = ((y + border) * nw + (x + border)) * 3;

            dst->data[dst_i + 0] = src->data[src_i + 0];
            dst->data[dst_i + 1] = src->data[src_i + 1];
            dst->data[dst_i + 2] = src->data[src_i + 2];
        }
    }

    return 1;
}