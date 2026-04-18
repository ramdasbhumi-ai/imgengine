// ./src/pipeline/canvas_free_buffer.c
#include "pipeline/canvas.h"

void img_canvas_free(img_canvas_t *canvas, img_slab_pool_t *pool)
{
    if (canvas && canvas->buf.data && pool)
    {
        img_slab_free(pool, canvas->buf.data);
        canvas->buf.data = NULL;
    }
}
