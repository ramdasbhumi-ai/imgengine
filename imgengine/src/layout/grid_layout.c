
// // #include "imgengine/image.h"

// // int layout_grid(img_t *canvas,
// //                 const img_t *photo,
// //                 int cols, int rows, int gap)
// // {

// //     int total_w = cols * photo->width + (cols - 1) * gap;
// //     int total_h = rows * photo->height + (rows - 1) * gap;

// //     int start_x = (canvas->width - total_w) / 2;
// //     int start_y = (canvas->height - total_h) / 2;

// //     for (int r = 0; r < rows; r++)
// //     {
// //         for (int c = 0; c < cols; c++)
// //         {

// //             int x0 = start_x + c * (photo->width + gap);
// //             int y0 = start_y + r * (photo->height + gap);

// //             for (int y = 0; y < photo->height; y++)
// //             {
// //                 for (int x = 0; x < photo->width; x++)
// //                 {

// //                     int src = (y * photo->width + x) * 3;
// //                     int dst = ((y0 + y) * canvas->width + (x0 + x)) * 3;

// //                     canvas->data[dst + 0] = photo->data[src + 0];
// //                     canvas->data[dst + 1] = photo->data[src + 1];
// //                     canvas->data[dst + 2] = photo->data[src + 2];
// //                 }
// //             }
// //         }
// //     }

// //     return 1;
// // }

// // src/layout/layout_grid.c

// #include "imgengine/image.h"

// int layout_grid(img_t *canvas,
//                 const img_t *photo,
//                 int cols, int rows, int gap)
// {
//     int pw = photo->width;
//     int ph = photo->height;

//     int padding = 20; // 🔥 default outer padding

//     int x = padding;
//     int y = padding;

//     for (int r = 0; r < rows; r++)
//     {
//         x = padding;

//         for (int c = 0; c < cols; c++)
//         {
//             // bounds check (very important)
//             if (x + pw > canvas->width || y + ph > canvas->height)
//                 return 0;

//             // copy image
//             for (int iy = 0; iy < ph; iy++)
//             {
//                 for (int ix = 0; ix < pw; ix++)
//                 {
//                     int src_idx = (iy * pw + ix) * 3;
//                     int dst_idx = ((y + iy) * canvas->width + (x + ix)) * 3;

//                     canvas->data[dst_idx + 0] = photo->data[src_idx + 0];
//                     canvas->data[dst_idx + 1] = photo->data[src_idx + 1];
//                     canvas->data[dst_idx + 2] = photo->data[src_idx + 2];
//                 }
//             }

//             x += pw + gap;
//         }

//         y += ph + gap;
//     }

//     return 1;
// }

// src/layout/grid_layout.c 

#include "imgengine/image.h"

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

// int layout_grid(img_t *canvas,
//                 const img_t *photo,
//                 int cols, int rows, int gap, int padding) // ✅ FIXED
// {
//     int pw = photo->width;
//     int ph = photo->height;

//     int x = padding;
//     int y = padding;

//     for (int r = 0; r < rows; r++)
//     {
//         x = padding;

//         for (int c = 0; c < cols; c++)
//         {
//             if (x + pw > canvas->width || y + ph > canvas->height)
//                 return 0;

//             for (int iy = 0; iy < ph; iy++)
//             {
//                 for (int ix = 0; ix < pw; ix++)
//                 {
//                     int src_idx = (iy * pw + ix) * 3;
//                     int dst_idx = ((y + iy) * canvas->width + (x + ix)) * 3;

//                     canvas->data[dst_idx + 0] = photo->data[src_idx + 0];
//                     canvas->data[dst_idx + 1] = photo->data[src_idx + 1];
//                     canvas->data[dst_idx + 2] = photo->data[src_idx + 2];
//                 }
//             }

//             x += pw + gap;
//         }

//         y += ph + gap;
//     }

//     return 1;
// }