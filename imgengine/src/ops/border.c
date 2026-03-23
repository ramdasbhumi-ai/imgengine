// #include "imgengine/image.h"
// #include <string.h>

// void img_add_border(img_t *img, int border)
// {

//     int nw = img->width + 2 * border;
//     int nh = img->height + 2 * border;

//     unsigned char *new_data = malloc(nw * nh * 3);

//     memset(new_data, 0, nw * nh * 3); // black

//     for (int y = 0; y < img->height; y++)
//     {
//         for (int x = 0; x < img->width; x++)
//         {

//             int src_i = (y * img->width + x) * 3;
//             int dst_i = ((y + border) * nw + (x + border)) * 3;

//             new_data[dst_i + 0] = img->data[src_i + 0];
//             new_data[dst_i + 1] = img->data[src_i + 1];
//             new_data[dst_i + 2] = img->data[src_i + 2];
//         }
//     }

//     img->width = nw;
//     img->height = nh;
//     img->data = new_data;
// }

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