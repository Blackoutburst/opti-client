#include <stdlib.h>

#include "devices/logicalDevice.h"
#include "devices/queueFamilies.h"
#include "devices/physicalDevice.h"
#include "renderer/windowSurface.h"
#include "utils/logger.h"

static VkDevice device = VK_NULL_HANDLE;

#define REQUIRED_EXTENSIONS_COUNT 1
static const I8* requiredExtensions[REQUIRED_EXTENSIONS_COUNT] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VkDevice logicalDeviceGet(void) {
    return device;
}

void logicalDeviceClean(void) {
    if (device != VK_NULL_HANDLE) {
        vkDestroyDevice(device, NULL);
    }
}

VkQueue logicalDeviceGetQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkQueueFlagBits queueType) {
    VkQueue graphicsQueue;
    vkGetDeviceQueue(logicalDevice, queueFamiliesGetType(physicalDevice, queueType), 0, &graphicsQueue);

    return graphicsQueue;
}

VkQueue logicalDeviceGetPresentationQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice) {
    VkQueue presentationQueue;
    vkGetDeviceQueue(logicalDevice, logicalDeviceGetSurfaceSupport(physicalDevice), 0, &presentationQueue);

    return presentationQueue;
}


U32 logicalDeviceGetSurfaceSupport(VkPhysicalDevice device) {
    U32 queueCount = queueFamiliesCount(device);
    VkQueueFamilyProperties* queues = malloc(sizeof(VkQueueFamilyProperties) * queueCount);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queues);

    for (U32 i = 0; i < queueCount; i++) {
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, windowSurfaceGet(), &presentSupport);
        if (presentSupport) return i;
    }

    return 0;
}

void logicalDeviceCreate(VkPhysicalDevice physicalDevice, VkDevice* logicalDevice) {
    const F32 queuePriority = 1.0;

    VkDeviceQueueCreateInfo graphicsQueueCreateInfo;
    graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphicsQueueCreateInfo.pNext = NULL;
    graphicsQueueCreateInfo.flags = 0;
    graphicsQueueCreateInfo.queueFamilyIndex = queueFamiliesGetType(physicalDevice, VK_QUEUE_GRAPHICS_BIT);
    graphicsQueueCreateInfo.queueCount = 1;
    graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceQueueCreateInfo presentQueueCreateInfo;
    presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    presentQueueCreateInfo.pNext = NULL;
    presentQueueCreateInfo.flags = 0;
    presentQueueCreateInfo.queueFamilyIndex = logicalDeviceGetSurfaceSupport(physicalDevice);
    presentQueueCreateInfo.queueCount = 1;
    presentQueueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = { VK_FALSE };

    VkDeviceQueueCreateInfo queues[2] = { graphicsQueueCreateInfo, presentQueueCreateInfo };

    U8 sameQueue = queueFamiliesGetType(physicalDevice, VK_QUEUE_GRAPHICS_BIT) == logicalDeviceGetSurfaceSupport(physicalDevice);

    VkDeviceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = sameQueue ? 1 : 2;
    createInfo.pQueueCreateInfos = sameQueue ? &graphicsQueueCreateInfo : queues;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = NULL;
    createInfo.enabledExtensionCount = REQUIRED_EXTENSIONS_COUNT;
    createInfo.ppEnabledExtensionNames = requiredExtensions;
    createInfo.pEnabledFeatures = &deviceFeatures;

    vkCreateDevice(physicalDevice, &createInfo, NULL, logicalDevice);
}

void logicalDeviceInit(void) {
    VkPhysicalDevice physicalDevice = physicalDeviceGet();

    if (physicalDevice != VK_NULL_HANDLE) {
        logicalDeviceCreate(physicalDevice, &device);
    }
}
