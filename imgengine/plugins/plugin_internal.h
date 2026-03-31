/* plugins/plugin_internal.h */
#ifndef IMGENGINE_PLUGINS_INTERNAL_H
#define IMGENGINE_PLUGINS_INTERNAL_H

#include "pipeline/pipeline.h"

// OpCodes (Standardized across the engine)
#define OP_RESIZE 0x01
#define OP_CROP 0x02
#define OP_BLEED 0x03
#define OP_PDF 0x04

// Plugin Registry Entry
typedef struct
{
    uint32_t op_code;
    const char *name;
    img_op_fn single_exec;
    img_batch_op_fn batch_exec;
} img_plugin_info_t;

// Global Registration Hook
void img_plugins_init_all(void);

#endif
