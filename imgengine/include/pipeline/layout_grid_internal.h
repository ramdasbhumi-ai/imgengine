// ./include/pipeline/layout_grid_internal.h
#ifndef IMGENGINE_PIPELINE_LAYOUT_GRID_INTERNAL_H
#define IMGENGINE_PIPELINE_LAYOUT_GRID_INTERNAL_H

#include "pipeline/layout.h"
#include "pipeline/layout_resize.h"

img_result_t img_layout_grid_prepare_scaled(
    const img_buffer_t *photo,
    const img_job_t *job,
    uint32_t pw,
    uint32_t ph,
    img_slab_pool_t *pool,
    img_buffer_t *scaled);

img_result_t img_layout_grid_alloc_cells(
    const img_job_t *job,
    img_arena_t *arena,
    img_cell_t **cells_out);

uint32_t img_layout_grid_place_cells(
    img_canvas_t *canvas,
    const img_buffer_t *scaled,
    const img_job_t *job,
    img_cell_t *cells);

#endif /* IMGENGINE_PIPELINE_LAYOUT_GRID_INTERNAL_H */
