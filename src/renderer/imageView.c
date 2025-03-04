#include <stdlib.h>

#include "renderer/imageView.h"
#include "renderer/swapChain.h"
#include "devices/logicalDevice.h"

static VkImageView* imageView = NULL;

VkImageView* imageViewGet(void) {
    return imageView;
}

void imageViewInit(void) {
    VkDevice device = logicalDeviceGet();
    U32 swapChainImagesCount = swapChainGetImagesCount();
    VkImage* swapChainImages = swapChainGetImages();
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
}

void imageViewClean(void) {
    VkDevice device = logicalDeviceGet();
    U32 swapChainImagesCount = swapChainGetImagesCount();

    for (U32 i = 0; i < swapChainImagesCount; i++) {
        vkDestroyImageView(device, imageView[i], NULL);
    }

    free(imageView);
}

