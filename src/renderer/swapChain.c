#include <stdlib.h>
#include "renderer/swapChain.h"
#include "renderer/windowSurface.h"
#include "devices/physicalDevice.h"
#include "devices/queueFamilies.h"
#include "devices/logicalDevice.h"
#include "window/window.h"
#include "utils/math.h"

static VkSwapchainKHR swapChain;

VkImage* swapChainGetImages(void) {
    U32 count = 0;
    vkGetSwapchainImagesKHR(logicalDeviceGet(), swapChain, &count, NULL);

    VkImage* data = malloc(sizeof(VkSurfaceFormatKHR) * count);
    vkGetSwapchainImagesKHR(logicalDeviceGet(), swapChain, &count, data);

    return data;
}

VkSwapchainKHR swapChainGet(void) {
    return swapChain;
}

VkSurfaceFormatKHR swapChainFormat(void) {
    U32 count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceGet(), windowSurfaceGet(), &count, NULL);

    VkSurfaceFormatKHR* data = malloc(sizeof(VkSurfaceFormatKHR) * count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceGet(), windowSurfaceGet(), &count, data);

    for (U32 i = 0; i < count; i++) {
        if (data[i].format == VK_FORMAT_B8G8R8A8_SRGB && data[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return data[i];
        }
    }

    return data[0];
}

VkPresentModeKHR swapChainPresentMode(void) {
    U32 count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceGet(), windowSurfaceGet(), &count, NULL);

    VkPresentModeKHR* data = malloc(sizeof(VkSurfaceFormatKHR) * count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceGet(), windowSurfaceGet(), &count, data);

    for (U32 i = 0; i < count; i++) {
        if (data[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return data[i];
        }
    }

    return data[0];
}

VkSurfaceCapabilitiesKHR swapChainCapabilities(void) {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDeviceGet(), windowSurfaceGet(), &capabilities);

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

void swapChainClean(void) {
    vkDestroySwapchainKHR(logicalDeviceGet(), swapChain, NULL);
}

void swapChainInit(void) {
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

    U32 queues[2] = { queueFamiliesGetType(physicalDeviceGet(), VK_QUEUE_GRAPHICS_BIT), logicalDeviceGetSurfaceSupport(physicalDeviceGet()) };
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

    vkCreateSwapchainKHR(logicalDeviceGet(), &createInfo, NULL, &swapChain);
}
