#include "renderer/vk.h"
#include "devices/devices.h"
#include "debug/logCallback.h"
#include "window/window.h"
#include "renderer/rendererInstance.h"
#include "renderer/windowSurface.h"
#include "utils/args.h"
#include "utils/logger.h"
#include "renderer/swapChain.h"
#include "renderer/imageView.h"
#include "renderer/shader.h"
#include "renderer/pipeline.h"

void vkInit(void) {
    rendererInstanceInit();
    
    VkInstance instance = rendererInstanceGetInstance();

    if (argsGetValidationLayers()) {
        logCallBackSet(instance);
    }
    
    windowSurfaceInit(instance, windowGetHandle());

    devicesInit();
    swapChainInit();
    imageViewInit();

    VkShaderModule vertexShader = shaderInit("./shader/triangleVert.spv");
    VkShaderModule fragmentShader = shaderInit("./shader/triangleFrag.spv");

    pipelineInit(vertexShader, fragmentShader);
}

void vkClean(void) {
    VkInstance instance = rendererInstanceGetInstance();

    imageViewClean();
    swapChainClean();
    devicesClean();
    windowSurfaceClean(instance);
    
    if (argsGetValidationLayers()) {
        logCallBackClean(instance);
    }
    
    rendererInstanceClean();
}
