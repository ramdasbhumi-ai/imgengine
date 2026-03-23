// #include "imgengine/image.h"

// void img_center_crop(const img_t *src, img_t *dst,
//                      int target_w, int target_h)
// {

//     float src_ratio = (float)src->width / src->height;
//     float dst_ratio = (float)target_w / target_h;

//     int crop_w, crop_h;
//     int x_off = 0, y_off = 0;

//     if (src_ratio > dst_ratio)
//     {
//         crop_h = src->height;
//         crop_w = (int)(crop_h * dst_ratio);
//         x_off = (src->width - crop_w) / 2;
//     }
//     else
//     {
//         crop_w = src->width;
//         crop_h = (int)(crop_w / dst_ratio);
//         y_off = (src->height - crop_h) / 2;
//     }

//     dst->width = crop_w;
//     dst->height = crop_h;
//     dst->channels = 3;
//     dst->data = src->data + (y_off * src->width + x_off) * 3;
// }

#include "imgengine/image.h"

int img_center_crop(const img_t *src, img_t *dst,
                    mem_pool_t *mp,
                    int target_w, int target_h)
{

    float src_ratio = (float)src->width / src->height;
    float dst_ratio = (float)target_w / target_h;

    int crop_w, crop_h;
    int x_off = 0, y_off = 0;

    if (src_ratio > dst_ratio)
    {
        crop_h = src->height;
        crop_w = (int)(crop_h * dst_ratio);
        x_off = (src->width - crop_w) / 2;
    }
    else
    {
        crop_w = src->width;
        crop_h = (int)(crop_w / dst_ratio);
        y_off = (src->height - crop_h) / 2;
    }

    if (!img_create(dst, mp, crop_w, crop_h, 3))
        return 0;

    for (int y = 0; y < crop_h; y++)
    {
        for (int x = 0; x < crop_w; x++)
        {

            int src_i = ((y + y_off) * src->width + (x + x_off)) * 3;
            int dst_i = (y * crop_w + x) * 3;

            dst->data[dst_i + 0] = src->data[src_i + 0];
            dst->data[dst_i + 1] = src->data[src_i + 1];
            dst->data[dst_i + 2] = src->data[src_i + 2];
        }
    }

    return 1;
}