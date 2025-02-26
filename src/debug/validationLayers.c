#include "debug/validationLayers.h"

#define LAYERS_COUNT 9

const static I8* layers[LAYERS_COUNT] = {
    "VK_LAYER_LUNARG_api_dump",
    "VK_LAYER_LUNARG_crash_diagnostic",
    "VK_LAYER_LUNARG_gfxreconstruct",
    "VK_LAYER_KHRONOS_profiles",
    "VK_LAYER_KHRONOS_shader_object",
    "VK_LAYER_KHRONOS_synchronization2",
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_monitor",
    "VK_LAYER_LUNARG_screenshot"
};

I32 validationLayersCount(void) {
    return LAYERS_COUNT;
}

const I8** validationLayersGet(void) {
    return layers;
}
