// imgengine/image.h

#ifndef IMG_IMAGE_H
#define IMG_IMAGE_H

#include "memory_pool.h"

typedef struct
{
    int width;
    int height;
    int channels;
    unsigned char *data;
} img_t;

int img_create(img_t *img, mem_pool_t *mp, int w, int h, int ch);

#endif