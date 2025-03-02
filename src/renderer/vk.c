#include "renderer/vk.h"
#include "devices/devices.h"
#include "debug/logCallback.h"
#include "window/window.h"
#include "renderer/rendererInstance.h"
#include "renderer/windowSurface.h"
#include "utils/args.h"
#include "utils/logger.h"
#include "renderer/swapChain.h"

void vkInit(void) {
    rendererInstanceInit();
    
    VkInstance instance = rendererInstanceGetInstance();
    
    windowSurfaceInit(instance, windowGetHandle());

    devicesInit();
    swapChainInit();
    

    if (argsGetValidationLayers()) {
        logCallBackSet(instance);
    }
}

void vkClean(void) {
    VkInstance instance = rendererInstanceGetInstance();

    if (argsGetValidationLayers()) {
        logCallBackClean(instance);
    }

    swapChainClean();
    devicesClean();
    windowSurfaceClean(instance);
    rendererInstanceClean();
}
