// // src/cli/main.c

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

// 🔥 NEW DEFAULTS (FIXED SYNTAX)
#define DEFAULT_BLEED 10
#define DEFAULT_CROP_MARK 20
#define DEFAULT_CROP_THICKNESS 2

static void print_usage()
{
    printf("\n📸 imgengine_cli — Professional Photo Layout Engine\n\n");

    printf("Usage:\n");
    printf("  imgengine_cli --input input.jpg --output output.png [options]\n\n");

    printf("Core Layout:\n");
    printf("  --cols N            number of columns (default: %d)\n", DEFAULT_COLS);
    printf("  --rows N            number of rows (default: %d)\n", DEFAULT_ROWS);
    printf("  --gap N             gap in px (default: %d)\n", DEFAULT_GAP);
    printf("  --padding N         page padding in px (default: %d)\n", DEFAULT_PADDING);

    printf("\nPhoto Settings:\n");
    printf("  --width F           photo width in cm (default: %.1f)\n", DEFAULT_W_CM);
    printf("  --height F          photo height in cm (default: %.1f)\n", DEFAULT_H_CM);
    printf("  --dpi N             print DPI (default: %d)\n", DEFAULT_DPI);
    printf("  --border N          border in px (default: %d)\n", DEFAULT_BORDER);

    printf("\nPrint Features:\n");
    printf("  --bleed N           bleed in px (default: %d)\n", DEFAULT_BLEED);
    printf("  --crop-mark N       crop mark length (default: %d)\n", DEFAULT_CROP_MARK);
    printf("  --crop-thick N      crop mark thickness (default: %d)\n", DEFAULT_CROP_THICKNESS);

    printf("\nExamples:\n");
    printf("  # Passport (6 photos)\n");
    printf("  imgengine_cli --input in.jpg --output out.png --cols 3 --rows 2\n\n");

    printf("  # Studio print (A4 optimized)\n");
    printf("  imgengine_cli --input in.jpg --output out.png \\\n");
    printf("      --cols 6 --rows 2 --gap 15 --padding 20 \\\n");
    printf("      --width 3.5 --height 3.0\n\n");

    printf("  # High quality with bleed + crop marks\n");
    printf("  imgengine_cli --input in.jpg --output out.png \\\n");
    printf("      --cols 6 --rows 2 --bleed 10 --crop-mark 25\n\n");
}

int main(int argc, char **argv)
{
    const char *input = NULL;
    const char *output = "output.png";

    int cols = DEFAULT_COLS;
    int rows = DEFAULT_ROWS;
    int gap = DEFAULT_GAP;
    int dpi = DEFAULT_DPI;
    int border = DEFAULT_BORDER;
    int padding = DEFAULT_PADDING;

    int bleed = DEFAULT_BLEED;
    int crop_mark = DEFAULT_CROP_MARK;
    int crop_thickness = DEFAULT_CROP_THICKNESS;

    float w_cm = DEFAULT_W_CM;
    float h_cm = DEFAULT_H_CM;

    if (argc == 1)
    {
        print_usage();
        return 0;
    }

    // =========================
    // ARG PARSER
    // =========================
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc)
            input = argv[++i];

        else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc)
            output = argv[++i];

        else if (strcmp(argv[i], "--cols") == 0 && i + 1 < argc)
            cols = atoi(argv[++i]);

        else if (strcmp(argv[i], "--rows") == 0 && i + 1 < argc)
            rows = atoi(argv[++i]);

        else if (strcmp(argv[i], "--gap") == 0 && i + 1 < argc)
            gap = atoi(argv[++i]);

        else if (strcmp(argv[i], "--dpi") == 0 && i + 1 < argc)
            dpi = atoi(argv[++i]);

        else if (strcmp(argv[i], "--width") == 0 && i + 1 < argc)
            w_cm = atof(argv[++i]);

        else if (strcmp(argv[i], "--height") == 0 && i + 1 < argc)
            h_cm = atof(argv[++i]);

        else if (strcmp(argv[i], "--border") == 0 && i + 1 < argc)
            border = atoi(argv[++i]);

        else if (strcmp(argv[i], "--padding") == 0 && i + 1 < argc)
            padding = atoi(argv[++i]);

        // 🔥 NEW FLAGS
        else if (strcmp(argv[i], "--bleed") == 0 && i + 1 < argc)
            bleed = atoi(argv[++i]);

        else if (strcmp(argv[i], "--crop-mark") == 0 && i + 1 < argc)
            crop_mark = atoi(argv[++i]);

        else if (strcmp(argv[i], "--crop-thick") == 0 && i + 1 < argc)
            crop_thickness = atoi(argv[++i]);

        else if (strcmp(argv[i], "--help") == 0)
        {
            print_usage();
            return 0;
        }

        else
        {
            printf("❌ Unknown argument: %s\n", argv[i]);
            print_usage();
            return 1;
        }
    }

    if (!input)
    {
        printf("❌ Error: --input is required\n");
        return 1;
    }

    // printf("Loading image: %s\n", input);

    img_ctx_t ctx;
    img_ctx_init(&ctx, 100 * 1024 * 1024);

    img_job_t job = {
        .photo_w_cm = w_cm,
        .photo_h_cm = h_cm,
        .dpi = dpi,
        .cols = cols,
        .rows = rows,
        .gap = gap,
        .border_px = border,
        .padding = padding,

        // 🔥 NEW
        .bleed_px = bleed,
        .crop_mark_px = crop_mark,
        .crop_thickness = crop_thickness,

        .mode = IMG_FILL,
        .bg_r = 255,
        .bg_g = 255,
        .bg_b = 255};

    printf("Layout: %dx%d | size=%.1fx%.1f cm | gap=%d padding=%d\n",
           cols, rows, w_cm, h_cm, gap, padding);

    int ret = imgengine_run(&ctx, input, output, &job);

    if (ret != 0)
        printf("❌ Failed with error code: %d\n", ret);
    else
        printf("✅ Output generated: %s\n", output);

    img_ctx_destroy(&ctx);
    return ret;
}
