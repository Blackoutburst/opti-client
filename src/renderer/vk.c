#include "renderer/vk.h"
#include "devices/devices.h"
#include "debug/logCallback.h"
#include "window/window.h"
#include "renderer/rendererInstance.h"
#include "renderer/windowSurface.h"
#include "utils/args.h"

void vkInit(void) {
    rendererInstanceInit();
    
    VkInstance instance = rendererInstanceGetInstance();
    
    windowSurfaceInit(instance, windowGetHandle());
    

    devicesInit();

    if (argsGetValidationLayers()) {
        logCallBackSet(instance);
    }
}

void vkClean(void) {
    VkInstance instance = rendererInstanceGetInstance();

    if (argsGetValidationLayers()) {
        logCallBackClean(instance);
    }

    devicesClean();
    windowSurfaceClean(instance);
    rendererInstanceClean();
}
