// ./include/cmd/imgengine/args.h

#ifndef IMGENGINE_CLI_ARGS_H
#define IMGENGINE_CLI_ARGS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    const char *input_path;
    const char *output_path;

    /* layout */
    uint32_t cols;
    uint32_t rows;
    uint32_t gap;
    uint32_t padding;

    /* photo */
    float photo_w_cm;
    float photo_h_cm;
    uint32_t dpi;
    uint32_t border;

    /* print */
    uint32_t bleed;
    uint32_t crop_mark;
    uint32_t crop_thickness;
    uint32_t crop_offset;

    /* runtime */
    uint32_t threads;

    bool verbose;

} img_cli_options_t;

int img_parse_args(int argc, char **argv, img_cli_options_t *opts);
void img_print_usage(const char *bin);

#endif