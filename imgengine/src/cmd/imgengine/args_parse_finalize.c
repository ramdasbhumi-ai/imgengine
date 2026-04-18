// ./src/cmd/imgengine/args_parse_finalize.c
#include "cmd/imgengine/args_internal.h"

#include <stdio.h>

int img_cli_finalize_options(img_cli_options_t *opts)
{
    if (!opts)
        return -1;

    if (!opts->input_path)
    {
        fprintf(stderr, "imgengine: missing required --input\n");
        return -1;
    }

    if (!opts->has_threads)
        opts->threads = 1;

    if (opts->quiet)
        opts->verbose = false;

    return 0;
}
