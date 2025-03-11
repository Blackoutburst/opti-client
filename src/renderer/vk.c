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
#include "renderer/framebuffer.h"
#include "renderer/renderPass.h"
#include "devices/physicalDevice.h"
#include "devices/logicalDevice.h"
#include "devices/commandPool.h"
#include "devices/commandBuffer.h"

static VkShaderModule vertexShader;
static VkShaderModule fragmentShader;
static VkPipelineLayout pipelineLayout;
static VkRenderPass renderPass;
static VkPipeline pipeline;
static VkSwapchainKHR swapchain;
static VkImageView* imageView;
static VkFramebuffer* frambuffers;
static VkCommandPool cmdPool;
static VkCommandBuffer cmdBuffer;

void vkDrawTriangle(void) {
    commandBufferBegin(cmdBuffer);
    renderPassBegin(renderPass, frambuffers, cmdBuffer, 0);

    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    VkExtent2D extends = swapChainExtend();
    
    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (F32)extends.width;
    viewport.height = (F32)extends.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

    VkRect2D scissor;
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent = extends;
    vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

    vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

    renderPassEnd(cmdBuffer);
    commandBufferEnd(cmdBuffer);
}
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
    
    frambuffers = framebufferInit(swapchain, renderPass, imageView);
    cmdPool = commandPoolInit();
    cmdBuffer = commandBufferInit(cmdPool);
}

void vkClean(void) {
    VkInstance instance = renderer();

    commandBufferClean(cmdBuffer, cmdPool);
    commandPoolClean(cmdPool);
    framebufferClean(swapchain, frambuffers);
    
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
