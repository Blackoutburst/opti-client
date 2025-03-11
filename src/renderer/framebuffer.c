#include <stdlib.h>
#include "renderer/framebuffer.h"
#include "renderer/swapchain.h"
#include "devices/devices.h"

void framebufferClean(VkSwapchainKHR swapchain, VkFramebuffer* framebuffers) {
    U32 bufferCount = swapChainGetImagesCount(swapchain);

    for (U32 i = 0; i < bufferCount; i++) {
        vkDestroyFramebuffer(devices()->logical, framebuffers[i], NULL);
    }
}

VkFramebuffer* framebufferInit(VkSwapchainKHR swapchain, VkRenderPass renderPass, VkImageView* imageView) {
    U32 imageCount = swapChainGetImagesCount(swapchain);
    VkFramebuffer* framebuffers = malloc(sizeof(VkFramebuffer) * imageCount);
    VkExtent2D extends = swapChainExtend();

    for (U32 i = 0; i < imageCount; i++) {
        VkImageView attachments[1] = { imageView[i] };

        VkFramebufferCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.renderPass = renderPass;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = attachments;
        createInfo.width = extends.width;
        createInfo.height = extends.height;
        createInfo.layers = 1;

        vkCreateFramebuffer(devices()->logical, &createInfo, NULL, &framebuffers[i]);
    }

    return framebuffers;
}
