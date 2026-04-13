// ./include/cmd/imgengine/job_builder.h

#ifndef IMGENGINE_JOB_BUILDER_H
#define IMGENGINE_JOB_BUILDER_H

#include "cmd/imgengine/args.h"
#include "api/v1/img_job.h"

int img_build_job(const img_cli_options_t *opts, img_job_t *job);

#endif