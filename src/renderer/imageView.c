#include <stdlib.h>

#include "renderer/imageView.h"
#include "renderer/swapchain.h"
#include "devices/devices.h"

void imageViewClean(VkImageView* imageView, VkSwapchainKHR swapchain) {
    VkDevice device = devices()->logical;
    U32 swapChainImagesCount = swapChainGetImagesCount(swapchain);

    for (U32 i = 0; i < swapChainImagesCount; i++) {
        vkDestroyImageView(device, imageView[i], NULL);
    }

    free(imageView);
}

VkImageView* imageViewInit(VkSwapchainKHR swapchain) {
    VkImageView* imageView;
    VkDevice device = devices()->logical;
    U32 swapChainImagesCount = swapChainGetImagesCount(swapchain);
    VkImage* swapChainImages = swapChainGetImages(swapchain);
    VkSurfaceFormatKHR surfaceFormat = swapChainFormat();
    VkFormat format = surfaceFormat.format;

    imageView = malloc(sizeof(VkImageView) * swapChainImagesCount);

    for (U32 i = 0; i < swapChainImagesCount; i++) {
        VkImageViewCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        vkCreateImageView(device, &createInfo, NULL, &imageView[i]);
    }

    return imageView;
}

