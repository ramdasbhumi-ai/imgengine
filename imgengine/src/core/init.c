/* src/core/init.c */
#include "api/v1/img_core.h"
#include "pipeline/jump_table.h"
#include "io/decoder/decoder.h"
#include "core/config.h"

img_result_t img_core_global_init()
{
    // 1. Load System Tuning Knobs
    img_config_load_defaults();

    // 2. Map Plugins into the Static Jump Table (O(1) Dispatch)
    img_pipeline_register_op(OP_RESIZE, plugin_resize_exec);
    img_pipeline_register_op(OP_CROP, plugin_crop_exec);
    img_pipeline_register_op(OP_GRAY, plugin_grayscale_exec);

    // 3. Warm up the CPU Dispatcher
    // This ensures we don't branch-check during the first request
    img_get_cpu_arch();

    return IMG_SUCCESS;
}
