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
#include "renderer/renderPass.h"

static VkShaderModule vertexShader;
static VkShaderModule fragmentShader;
static VkPipelineLayout pipelineLayout;
static VkRenderPass renderPass;
static VkPipeline pipeline;

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

    vertexShader = shaderInit("./shader/triangleVert.spv");
    fragmentShader = shaderInit("./shader/triangleFrag.spv");
    pipelineLayout = pipelineCreateLayout();
    renderPass = renderPassInit();
    pipeline = pipelineInit(pipelineLayout, vertexShader, fragmentShader, renderPass);
}

void vkClean(void) {
    VkInstance instance = rendererInstanceGetInstance();

    shaderClean(vertexShader);
    shaderClean(fragmentShader);
    pipelineLayoutClean(pipelineLayout);
    renderPassClean(renderPass);
    pipelineClean(pipeline);
    imageViewClean();
    swapChainClean();
    devicesClean();
    windowSurfaceClean(instance);
    
    if (argsGetValidationLayers()) {
        logCallBackClean(instance);
    }
    
    rendererInstanceClean();
}
