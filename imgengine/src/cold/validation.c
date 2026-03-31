/* src/cold/validation.c */
#include "validation.h"

img_result_t img_validate_header(uint32_t w, uint32_t h, uint32_t channels)
{
    if (w == 0 || h == 0 || w > MAX_IMG_DIM || h > MAX_IMG_DIM)
        return IMG_ERR_FORMAT;
    if (channels != 3 && channels != 4)
        return IMG_ERR_FORMAT;
    return IMG_SUCCESS;
}
