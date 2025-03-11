#include "renderer/vk.h"
#include "devices/devices.h"
#include "debug/logCallback.h"
#include "window/window.h"
#include "renderer/rendererInstance.h"
#include "renderer/windowSurface.h"
#include "utils/args.h"
#include "utils/logger.h"
#include "renderer/swapchain.h"
#include "renderer/imageView.h"
#include "renderer/shader.h"
#include "renderer/pipeline.h"
#include "renderer/renderPass.h"
#include "devices/physicalDevice.h"
#include "devices/logicalDevice.h"

static VkShaderModule vertexShader;
static VkShaderModule fragmentShader;
static VkPipelineLayout pipelineLayout;
static VkRenderPass renderPass;
static VkPipeline pipeline;
static VkSwapchainKHR swapchain;
static VkImageView* imageView;

void vkInit(void) {
    rendererInstanceInit();
    
    VkInstance instance = renderer();

    if (argsGetValidationLayers()) {
        logCallBackSet(instance);
    }
    
    windowSurfaceInit(instance, windowGetHandle());

    VkPhysicalDevice physical = physicalDeviceInit();
    VkDevice logical = logicalDeviceInit(physical);
    devicesInit(physical, logical);
    
    swapchain = swapChainInit();
    imageView = imageViewInit(swapchain);

    vertexShader = shaderInit("./shader/triangleVert.spv");
    fragmentShader = shaderInit("./shader/triangleFrag.spv");
    pipelineLayout = pipelineCreateLayout();
    renderPass = renderPassInit();
    pipeline = pipelineInit(pipelineLayout, vertexShader, fragmentShader, renderPass);
}

void vkClean(void) {
    VkInstance instance = renderer();

    shaderClean(vertexShader);
    shaderClean(fragmentShader);
    pipelineLayoutClean(pipelineLayout);
    renderPassClean(renderPass);
    pipelineClean(pipeline);
    imageViewClean(imageView, swapchain);
    swapChainClean(swapchain);
    devicesClean();
    windowSurfaceClean(instance);
    
    if (argsGetValidationLayers()) {
        logCallBackClean(instance);
    }
    
    rendererInstanceClean();
}
