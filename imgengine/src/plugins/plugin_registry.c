// plugins/plugin_registry.c

#include "api/v1/img_plugin_api.h"
#include "pipeline/jump_table.h"
#include "core/opcodes.h"

#include <dlfcn.h>
#include <stdio.h>

#define MAX_PLUGINS 128

static const img_plugin_descriptor_t *g_plugins[MAX_PLUGINS];
static uint32_t g_plugin_count = 0;

static void register_plugin(const img_plugin_descriptor_t *p)
{
    if (!p)
        return;

    if (p->abi_version != IMG_PLUGIN_ABI_VERSION)
    {
        fprintf(stderr, "ABI mismatch for plugin: %s\n", p->name);
        return;
    }

    g_plugins[g_plugin_count++] = p;

    if (p->capabilities & IMG_CAP_SINGLE)
        img_register_op(p->op_code, p->single_exec, NULL);

    if (p->capabilities & IMG_CAP_BATCH)
        img_register_batch_op(p->op_code, p->batch_exec);
}

// 🔥 STATIC LINKED PLUGINS
extern const img_plugin_descriptor_t *img_plugin_get_descriptor(void);

void img_plugins_init_all(void)
{
    register_plugin(img_plugin_get_descriptor());
}