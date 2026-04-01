// /* src/core/image.c */
// #include "image.h"

// img_buffer_t img_buffer_create(uint8_t *memory, uint32_t w, uint32_t h, uint32_t c)
// {
//     img_buffer_t buf;
//     buf.width = w;
//     buf.height = h;
//     buf.channels = c;

//     // KERNEL-GRADE: Align stride to 64 bytes (Cache Line)
//     // Ensures SIMD kernels can load rows without unaligned penalties
//     uint32_t row_bytes = w * c;
//     buf.stride = (row_bytes + 63) & ~63;

//     buf.data = memory;
//     return buf;
// }
