// ./src/api/api_encoded.c
#include "api/v1/img_api.h"

#include <stdlib.h>

void img_encoded_free(uint8_t *ptr)
{
    if (ptr)
        free(ptr);
}
