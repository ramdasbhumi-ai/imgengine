/* security/input_validator.c */
#include "security/input_validator.h"

img_result_t img_security_validate_request(uint32_t w, uint32_t h, size_t file_size)
{
    // 1. Sanity: No zero-size or negative-cast dimensions
    if (w == 0 || h == 0 || w > 16384 || h > 16384)
        return IMG_ERR_SECURITY;

    // 2. Ratio Check: Protect against "Pixel Bombs"
    // If a 1KB file claims to be 4K resolution, it's likely a malicious zip-bomb.
    uint64_t total_pixels = (uint64_t)w * h;
    if (total_pixels > (file_size * 500))
        return IMG_ERR_SECURITY;

    return IMG_SUCCESS;
}
