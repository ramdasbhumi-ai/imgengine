/* io/encoder/output_writer.c */
#include "io/io_vfs.h"
#include <turbojpeg.h>

img_result_t img_encode_to_mem(img_buffer_t *buf, uint8_t **out_ptr, size_t *out_size)
{
    tjhandle handle = tjInitCompress();
    // Compress directly from the 64-byte aligned slab
    tjCompress2(handle, buf->data, buf->width, buf->stride, buf->height,
                TJPF_RGB, out_ptr, out_size, TJSAMP_444, 85, TJFLAG_FASTDCT);
    tjDestroy(handle);
    return IMG_SUCCESS;
}
