#include "renderer/vk.h"
#include "devices/devices.h"
#include "debug/logCallback.h"
#include "renderer/rendererInstance.h"
#include "utils/args.h"

void vkInit(void) {
    rendererInstanceInit();

    devicesInit();

    if (argsGetValidationLayers()) {
        logCallBackSet(rendererInstanceGetInstance());
    }
}

void vkClean(void) {
    if (argsGetValidationLayers()) {
        logCallBackClean(rendererInstanceGetInstance());
    }

    devicesClean();
    rendererInstanceClean();
}
