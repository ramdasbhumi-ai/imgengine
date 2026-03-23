// imgengine/layout.h

#ifndef IMG_LAYOUT_H
#define IMG_LAYOUT_H

#include "image.h"

int layout_grid(img_t *canvas,
                const img_t *photo,
                int cols, int rows, int gap, int padding);

void layout_autofit(int canvas_w, int canvas_h,
                    int pw, int ph,
                    int gap, int padding,
                    int *cols, int *rows);

#endif