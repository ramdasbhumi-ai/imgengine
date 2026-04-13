// ./src/cmd/imgengine/job_builder.c

#include "cmd/imgengine/job_builder.h"
#include <string.h>

int img_build_job(const img_cli_options_t *opts, img_job_t *job)
{
    if (!opts || !job)
        return -1;

    img_job_defaults(job);

    /* layout */
    if (opts->cols)
        job->cols = opts->cols;
    if (opts->rows)
        job->rows = opts->rows;
    if (opts->gap)
        job->gap = opts->gap;
    if (opts->padding)
        job->padding = opts->padding;

    /* photo */
    if (opts->photo_w_cm)
        job->photo_w_cm = opts->photo_w_cm;
    if (opts->photo_h_cm)
        job->photo_h_cm = opts->photo_h_cm;
    if (opts->dpi)
        job->dpi = opts->dpi;
    if (opts->border)
        job->border_px = opts->border;

    /* print */
    if (opts->bleed)
        job->bleed_px = opts->bleed;
    if (opts->crop_mark)
        job->crop_mark_px = opts->crop_mark;
    if (opts->crop_thickness)
        job->crop_thickness = opts->crop_thickness;
    if (opts->crop_offset)
        job->crop_offset_px = opts->crop_offset;

    return 0;
}