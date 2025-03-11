#include <stdlib.h>
#include "renderer/swapchain.h"
#include "renderer/windowSurface.h"
#include "devices/devices.h"
#include "devices/logicalDevice.h"
#include "devices/queueFamilies.h"
#include "window/window.h"
#include "utils/math.h"

VkImage* swapChainGetImages(VkSwapchainKHR swapchain) {
    U32 count = swapChainGetImagesCount(swapchain);
    VkImage* data = malloc(sizeof(VkSurfaceFormatKHR) * count);
    vkGetSwapchainImagesKHR(devices()->logical, swapchain, &count, data);

    return data;
}

U32 swapChainGetImagesCount(VkSwapchainKHR swapchain) {
    U32 count = 0;
    vkGetSwapchainImagesKHR(devices()->logical, swapchain, &count, NULL);

    return count;
}

VkSurfaceFormatKHR swapChainFormat(void) {
    U32 count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(devices()->physical, windowSurfaceGet(), &count, NULL);

    VkSurfaceFormatKHR* data = malloc(sizeof(VkSurfaceFormatKHR) * count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(devices()->physical, windowSurfaceGet(), &count, data);

    for (U32 i = 0; i < count; i++) {
        if (data[i].format == VK_FORMAT_B8G8R8A8_SRGB && data[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return data[i];
        }
    }

    return data[0];
}

VkPresentModeKHR swapChainPresentMode(void) {
    U32 count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(devices()->physical, windowSurfaceGet(), &count, NULL);

    VkPresentModeKHR* data = malloc(sizeof(VkSurfaceFormatKHR) * count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(devices()->physical, windowSurfaceGet(), &count, data);

    for (U32 i = 0; i < count; i++) {
        if (data[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return data[i];
        }
    }

    return data[0];
}

VkSurfaceCapabilitiesKHR swapChainCapabilities(void) {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices()->physical, windowSurfaceGet(), &capabilities);

    return capabilities;
}

VkExtent2D swapChainExtend(void) {
    VkSurfaceCapabilitiesKHR capabilities = swapChainCapabilities();

    if (capabilities.currentExtent.width != 0xFFFFFFFF)
        return capabilities.currentExtent;

    I32 width = 0;
    I32 height = 0;
    glfwGetFramebufferSize(windowGetHandle(), &width, &height);

    return (VkExtent2D) {
        clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
        clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
    };
}

void swapChainClean(VkSwapchainKHR swapchain) {
    vkDestroySwapchainKHR(devices()->logical, swapchain, NULL);
}

VkSwapchainKHR swapChainInit(void) {
    VkSwapchainKHR swapchain;
    VkSurfaceCapabilitiesKHR capabilities = swapChainCapabilities();
    VkSurfaceFormatKHR surfaceFormat = swapChainFormat();
    VkPresentModeKHR presentMode = swapChainPresentMode();
    VkExtent2D extent = swapChainExtend();

    U32 imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        imageCount = capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.surface = windowSurfaceGet();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    U32 queues[2] = { queueFamiliesGetType(devices()->physical, VK_QUEUE_GRAPHICS_BIT), logicalDeviceGetSurfaceSupport(devices()->physical) };
    U8 sameQueue = queues[0] == queues[1];

    if (sameQueue) {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = NULL;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queues;
    }

    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    vkCreateSwapchainKHR(devices()->logical, &createInfo, NULL, &swapchain);

    return swapchain;
}
