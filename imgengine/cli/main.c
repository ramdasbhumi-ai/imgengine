// src/cli/main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imgengine/api.h"

#define DEFAULT_DPI 300
#define DEFAULT_COLS 2
#define DEFAULT_ROWS 3
#define DEFAULT_GAP 15
#define DEFAULT_BORDER 2
#define DEFAULT_W_CM 4.5f
#define DEFAULT_H_CM 3.5f
#define DEFAULT_PADDING 20
#define BLEED_PX = 10
#define CROP_MARK_PX = 20
#define CROP_CHICKNESS = 2

static void print_usage()
{
    printf("Usage:\n");
    printf("  imgengine_cli --input input.jpg --output output.png [options]\n\n");

    printf("Options:\n");
    printf("  --cols N        number of columns (default 2)\n");
    printf("  --rows N        number of rows (default 3)\n");
    printf("  --gap N         gap in pixels (default 15)\n");
    printf("  --dpi N         DPI (default 300)\n");
    printf("  --width F       photo width in cm (default 4.5)\n");
    printf("  --height F      photo height in cm (default 3.5)\n");
    printf("  --border N      border in px (default 2)\n");
    printf("  --padding N      padding in px (default 20)\n");
    // printf("AutoFit → cols: %d rows: %d\n", cols, rows);
}

int main(int argc, char **argv)
{
    int auto_fit = 0;
    const char *input = NULL;
    const char *output = "output.png";

    int cols = DEFAULT_COLS;
    int rows = DEFAULT_ROWS;
    int gap = DEFAULT_GAP;
    int dpi = DEFAULT_DPI;
    int border = DEFAULT_BORDER;
    int padding = DEFAULT_PADDING;

    float w_cm = DEFAULT_W_CM;
    float h_cm = DEFAULT_H_CM;

    // =========================
    // ARG PARSING
    // =========================
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc)
        {
            input = argv[++i];
        }
        else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc)
        {
            output = argv[++i];
        }
        else if (strcmp(argv[i], "--cols") == 0 && i + 1 < argc)
        {
            cols = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--rows") == 0 && i + 1 < argc)
        {
            rows = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--gap") == 0 && i + 1 < argc)
        {
            gap = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--dpi") == 0 && i + 1 < argc)
        {
            dpi = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--width") == 0 && i + 1 < argc)
        {
            w_cm = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "--height") == 0 && i + 1 < argc)
        {
            h_cm = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "--border") == 0 && i + 1 < argc)
        {
            border = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--padding") == 0 && i + 1 < argc)
        {
            padding = atoi(argv[++i]);
        }

        else if (strcmp(argv[i], "--auto-fit") == 0)
        {
            auto_fit = 1;
        }

        else
        {
            printf("Unknown argument: %s\n\n", argv[i]);
            print_usage();
            return 1;
        }
    }

    if (!input)
    {
        printf("Error: --input is required\n\n");
        print_usage();
        return 1;
    }

    // =========================
    // ENGINE SETUP
    // =========================
    img_ctx_t ctx;
    img_ctx_init(&ctx, 100 * 1024 * 1024); // safer pool

    img_job_t job = {
        .photo_w_cm = w_cm,
        .photo_h_cm = h_cm,
        .dpi = dpi,
        .cols = cols,
        .rows = rows,
        .gap = gap,
        .border_px = border,
        .padding = padding, // 🔥 ADD THIS LINE
        .auto_fit = auto_fit,
        .mode = IMG_FILL,
        .bg_r = 255,
        .bg_g = 255,
        .bg_b = 255};

    // =========================
    // RUN ENGINE
    // =========================
    int ret = imgengine_run(&ctx, input, output, &job);

    if (ret != 0)
    {
        printf("❌ Failed with error code: %d\n", ret);
    }
    else
    {
        printf("✅ Output generated: %s\n", output);
    }

    img_ctx_destroy(&ctx);
    return ret;
}