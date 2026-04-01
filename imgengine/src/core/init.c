#include "core/init.h"
#include "pipeline/registry.h"
#include "plugins/plugin_internal.h"
#include "observability/metrics.h"

void img_engine_bootstrap(void)
{
    // 1. Metrics
    img_metrics_init();

    // 2. Hardware kernels
    img_registry_init_hardware(img_detect_cpu());

    // 3. Plugins
    img_plugins_init_all();
}