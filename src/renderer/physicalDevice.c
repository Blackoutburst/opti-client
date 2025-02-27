#include <stdlib.h>

#include "utils/logger.h"
#include "renderer/physicalDevice.h"
#include "renderer/queueFamilies.h"

static VkPhysicalDevice primaryDevice = VK_NULL_HANDLE;
static VkPhysicalDevice secondaryDevice = VK_NULL_HANDLE;

static const VkPhysicalDeviceType devicePriorityList[5] = {
    VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
    VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
    VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
    VK_PHYSICAL_DEVICE_TYPE_CPU,
    VK_PHYSICAL_DEVICE_TYPE_OTHER
};

VkPhysicalDevice physicalDeviceGetPrimary(void) {
    return primaryDevice;
}

VkPhysicalDevice physicalDeviceGetSecondary(void) {
    return secondaryDevice;
}

void physicalDeviceInit(VkInstance instance) {
    physicalDeviceList(instance);

    U32 deviceCount = physicalDeviceCount(instance);
    VkPhysicalDeviceProperties primaryDeviceProperties;
    VkPhysicalDeviceProperties secondaryDeviceProperties;

    U32 i = 0;
    while (primaryDevice == VK_NULL_HANDLE && i < 5) {
        primaryDevice = physicalDeviceGetDevice(instance, devicePriorityList[i++], 0);
    }
    vkGetPhysicalDeviceProperties(primaryDevice, &primaryDeviceProperties);


    if (deviceCount > 1) {
        i = 0;
        while (secondaryDevice == VK_NULL_HANDLE && i < 5) {
            secondaryDevice = physicalDeviceGetDevice(instance, devicePriorityList[i++], primaryDeviceProperties.deviceID);
        }
        vkGetPhysicalDeviceProperties(secondaryDevice, &secondaryDeviceProperties);
    }

    if (secondaryDevice != VK_NULL_HANDLE) {
        logI("GPU's selected | Primary: %s (%s) | Secondary %s (%s)", primaryDeviceProperties.deviceName, physicalDeviceTypeName(primaryDeviceProperties.deviceType), secondaryDeviceProperties.deviceName, physicalDeviceTypeName(secondaryDeviceProperties.deviceType));
    } else {
        logI("GPU's selected | Primary: %s (%s) | Secondary: NONE", primaryDeviceProperties.deviceName, physicalDeviceTypeName(primaryDeviceProperties.deviceType));
    }
}

const I8* physicalDeviceTypeName(VkPhysicalDeviceType type) {
    switch (type) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER: return "OTHER";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "INTEGRATED_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "DISCRETE_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "VIRTUAL_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
        default: return "UNKOWN";
    }
}

U32 physicalDeviceCount(VkInstance instance) {
    U32 count = 0;

    vkEnumeratePhysicalDevices(instance, &count, NULL);

    return count;
}

void physicalDeviceList(VkInstance instance) {
    U32 deviceCount = physicalDeviceCount(instance);
    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);

    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    logI("Physical devices found: %i", deviceCount);

    for (U32 i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
        logI("Device: %s | Type: %s | Id: %u", deviceProperties.deviceName, physicalDeviceTypeName(deviceProperties.deviceType), deviceProperties.deviceID);
    }

    free(devices);
}

VkPhysicalDevice physicalDeviceGetDevice(VkInstance instance, U8 desiredType, U32 primaryId) {
    U32 deviceCount = physicalDeviceCount(instance);
    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);

    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    for (U32 i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);

        if (deviceProperties.deviceID == primaryId) continue;
        if (!queueFamiliesFindType(devices[i], VK_QUEUE_GRAPHICS_BIT)) continue;

        if (deviceProperties.deviceType == desiredType) return devices[i];
    }

    return VK_NULL_HANDLE;
}
