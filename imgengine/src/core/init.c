// /* src/core/init.c */
// #include "api/v1/img_core.h"
// #include "pipeline/jump_table.h"
// #include "io/decoder/decoder.h"
// #include "core/config.h"
// #include "observability/logger.h"

// img_result_t img_core_global_init()
// {
//     // Start the async logger first
//     img_log_init("imgengine.log");
//     img_log_write(LOG_INFO, "[CORE] Initializing imgengine v1.0...");

//     // 1. Load System Tuning Knobs
//     img_config_load_defaults();

//     // 2. Map Plugins into the Static Jump Table (O(1) Dispatch)
//     img_pipeline_register_op(OP_RESIZE, plugin_resize_exec);
//     img_pipeline_register_op(OP_CROP, plugin_crop_exec);
//     img_pipeline_register_op(OP_GRAY, plugin_grayscale_exec);

//     // 3. Warm up the CPU Dispatcher
//     // This ensures we don't branch-check during the first request
//     img_get_cpu_arch();

//     return IMG_SUCCESS;
// }

// /* src/core/init.c (Updated) */
// #include "api/v1/img_core.h"
// #include "observability/metrics.h"
// #include "observability/logger.h"
// #include "observability/tracing.h"

// img_result_t img_core_global_init() {
//     // 1. Start the Async Logger (Cold Path)
//     img_log_init("imgengine.log");
//     img_log_write(LOG_INFO, "Kernel-Grade Engine Bootstrapping...");

//     // 2. Initialize Atomic Metrics Scoreboard
//     // No allocation here, just zeroing the memory
//     img_metrics_init();

//     // 3. Detect Architecture & Map Jump Table
//     img_arch_init_dispatch();
//     img_registry_init_hardware(img_get_cpu_arch());

//     img_log_write(LOG_INFO, "Hardware Dispatcher Ready. P99 Monitoring Active.");
//     return IMG_SUCCESS;
// }

/* src/core/init.c */
#include "api/v1/img_core.h"
#include "core/config.h"
#include "pipeline/registry.h"
#include "pipeline/jump_table.h"
#include "observability/logger.h"
#include "observability/metrics.h"
#include "observability/tracing.h"

img_result_t img_core_global_init()
{
    // 1. ASYNC LOGGING (First, to catch boot errors)
    img_log_init("imgengine.log");
    img_log_write(LOG_INFO, "[CORE] Kernel-Grade Engine Bootstrapping...");

    // 2. METRICS (Zero-allocation scoreboard)
    img_metrics_init();

    // 3. TUNING (Load SaaS knobs)
    img_config_load_defaults();

    // 4. HARDWARE DISPATCH (CPUID -> Jump Table)
    cpu_arch_t arch = img_get_cpu_arch();
    img_registry_init_hardware(arch);

    img_log_write(LOG_INFO, "[CORE] Hardware Dispatcher Ready. P99 Monitoring Active.");
    return IMG_SUCCESS;
}
