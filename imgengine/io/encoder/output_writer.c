// /* io/encoder/output_writer.c */
// #include "io/io_vfs.h"
// #include <turbojpeg.h>

// img_result_t img_encode_to_mem(img_buffer_t *buf, uint8_t **out_ptr, size_t *out_size)
// {
//     tjhandle handle = tjInitCompress();
//     // Compress directly from the 64-byte aligned slab
//     tjCompress2(handle, buf->data, buf->width, buf->stride, buf->height,
//                 TJPF_RGB, out_ptr, out_size, TJSAMP_444, 85, TJFLAG_FASTDCT);
//     tjDestroy(handle);
//     return IMG_SUCCESS;
// }

/* io/encoder/output_writer.c */
#include "io/io_vfs.h"
#include "core/image.h" // CRITICAL: Defines img_buffer_t
#include <turbojpeg.h>

img_result_t img_encode_to_mem(img_buffer_t *buf, uint8_t **out_ptr, size_t *out_size)
{
    tjhandle handle = tjInitCompress();

    // We use buf->stride here. TurboJPEG will respect our 64-byte alignment
    // ensuring the encoder read is as fast as the SIMD write.
    int ret = tjCompress2(handle, buf->data, buf->width, buf->stride, buf->height,
                          TJPF_RGB, out_ptr, out_size, TJSAMP_444, 85, TJFLAG_FASTDCT);

    tjDestroy(handle);

    return (ret == 0) ? IMG_SUCCESS : IMG_ERR_IO;
}
