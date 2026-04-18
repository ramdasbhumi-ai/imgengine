// ./src/pipeline/layout_grid_prepare_scaled.c
#define _GNU_SOURCE

#include "pipeline/layout_grid_internal.h"

img_result_t img_layout_grid_prepare_scaled(
    const img_buffer_t *photo,
    const img_job_t *job,
    uint32_t pw,
    uint32_t ph,
    img_slab_pool_t *pool,
    img_buffer_t *scaled)
{
    if (job->mode == IMG_FIT)
    {
        return img_layout_prepare_fit(photo, scaled, pw, ph, pool,
                                      job->bg_r, job->bg_g, job->bg_b);
    }

    return img_layout_prepare_fill(photo, scaled, pw, ph, pool);
}
