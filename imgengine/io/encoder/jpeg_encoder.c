#include <turbojpeg.h>
#include "core/image.h"
#include "api/v1/img_error.h"
#include <stddef.h>

img_result_t img_encode_jpeg(
    img_buffer_t *buf,
    uint8_t **out,
    size_t *out_size)
{
    tjhandle handle = tjInitCompress();
    if (!handle)
        return IMG_ERR_IO;

    int ret = tjCompress2(
        handle,
        buf->data,
        buf->width,
        buf->stride,
        buf->height,
        TJPF_RGB,
        out,
        out_size,
        TJSAMP_444,
        85,
        TJFLAG_FASTDCT);

    tjDestroy(handle);

    return (ret == 0) ? IMG_SUCCESS : IMG_ERR_IO;
}