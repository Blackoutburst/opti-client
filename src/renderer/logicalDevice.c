#include "renderer/logicalDevice.h"
#include "renderer/queueFamilies.h"
#include "renderer/physicalDevice.h"
#include "utils/logger.h"

static VkDevice primaryDevice = VK_NULL_HANDLE;
static VkDevice secondaryDevice = VK_NULL_HANDLE;

VkDevice logicalDeviceGetPrimary(void) {
    return primaryDevice;
}

VkDevice logicalDeviceGetSecondary(void) {
    return secondaryDevice;
}

void logicalDeviceClean(void) {
    if (primaryDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(primaryDevice, NULL);
    }

    if (secondaryDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(secondaryDevice, NULL);
    }
}

void logicalDeviceCreate(VkPhysicalDevice physicalDevice, VkDevice* logicalDevice) {
    const F32 queuePriority = 1.0;

    VkDeviceQueueCreateInfo queueCreateInfo;
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = NULL;
    queueCreateInfo.flags = 0;
    queueCreateInfo.queueFamilyIndex = queueFamiliesGetType(physicalDevice, VK_QUEUE_GRAPHICS_BIT);
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = { VK_FALSE };

    VkDeviceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = NULL;
    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = NULL;
    createInfo.pEnabledFeatures = &deviceFeatures;

    vkCreateDevice(physicalDevice, &createInfo, NULL, logicalDevice);
}

void logicalDeviceInit(void) {
    VkPhysicalDevice primaryPhysicalDevice = physicalDeviceGetPrimary();
    VkPhysicalDevice secondaryPhysicalDevice = physicalDeviceGetSecondary();

    if (primaryPhysicalDevice != VK_NULL_HANDLE) {
        logicalDeviceCreate(primaryPhysicalDevice, &primaryDevice);
    }

    if (secondaryPhysicalDevice != VK_NULL_HANDLE) {
        logicalDeviceCreate(secondaryPhysicalDevice, &secondaryDevice);
    }
}
