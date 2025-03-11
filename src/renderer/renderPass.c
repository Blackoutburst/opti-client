#include "renderer/renderPass.h"
#include "renderer/swapchain.h"
#include "devices/devices.h"

void renderPassEnd(VkCommandBuffer cmdBuffer) {
    vkCmdEndRenderPass(cmdBuffer);
}

void renderPassBegin(VkRenderPass renderPass, VkFramebuffer* framebuffers, VkCommandBuffer cmdBuffer, U32 imageIndex) {
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    VkExtent2D extends = swapChainExtend();

    VkRenderPassBeginInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    createInfo.pNext = NULL;
    createInfo.renderPass = renderPass;
    createInfo.framebuffer = framebuffers[imageIndex];
    createInfo.renderArea.offset = (VkOffset2D){0, 0};
    createInfo.renderArea.extent = extends;
    createInfo.clearValueCount = 1;
    createInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(cmdBuffer, &createInfo, VK_SUBPASS_CONTENTS_INLINE);
}


void renderPassClean(VkRenderPass renderPass) {
    vkDestroyRenderPass(devices()->logical, renderPass, NULL);
}

VkRenderPass renderPassInit(void) {
    VkSurfaceFormatKHR surfaceFormat = swapChainFormat();
    VkFormat format = surfaceFormat.format;

    VkAttachmentDescription colorAttachment;
    colorAttachment.flags = 0;
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = NULL;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pResolveAttachments = NULL;
    subpass.pDepthStencilAttachment = NULL;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = NULL;

    VkRenderPass renderPass;
    VkRenderPassCreateInfo renderPassInfo;
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.flags = 0;
    renderPassInfo.pNext = NULL;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 0;
    renderPassInfo.pDependencies = NULL;
    
    vkCreateRenderPass(devices()->logical, &renderPassInfo, NULL, &renderPass);

    return renderPass;
}

