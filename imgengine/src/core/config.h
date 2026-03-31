/* src/core/config.h */
typedef struct
{
    uint32_t batch_size;      // Default: 8
    uint32_t prefetch_stride; // Default: 2 (cache lines)
    bool use_hugepages;       // Default: true
} img_config_t;

/* src/core/config.c */
img_config_t g_config = {
    .batch_size = 8,
    .prefetch_stride = 2,
    .use_hugepages = true};

void img_config_load_defaults()
{
    // Potentially load from environment variables for SaaS scaling
}
