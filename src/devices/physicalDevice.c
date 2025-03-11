#include <stdlib.h>
#include <string.h>

#include "utils/logger.h"
#include "devices/physicalDevice.h"
#include "devices/queueFamilies.h"
#include "renderer/windowSurface.h"
#include "renderer/rendererInstance.h"

#define PHYSICAL_DEVICE_TYPE_COUNT 5

#define REQUIRED_EXTENSIONS_COUNT 1
static const I8* requiredExtensions[REQUIRED_EXTENSIONS_COUNT] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static const VkPhysicalDeviceType devicePriorityList[PHYSICAL_DEVICE_TYPE_COUNT] = {
    VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
    VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
    VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
    VK_PHYSICAL_DEVICE_TYPE_CPU,
    VK_PHYSICAL_DEVICE_TYPE_OTHER
};

VkPhysicalDeviceProperties physicalDeviceGetProperties(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    return properties;
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

U32 physicalDeviceCount(void) {
    U32 count = 0;

    vkEnumeratePhysicalDevices(renderer(), &count, NULL);

    return count;
}

void physicalDeviceList(void) {
    U32 deviceCount = physicalDeviceCount();
    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);

    vkEnumeratePhysicalDevices(renderer(), &deviceCount, devices);

    logI("Physical devices found: %i", deviceCount);

    for (U32 i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
        logI("Device: %s | Type: %s", deviceProperties.deviceName, physicalDeviceTypeName(deviceProperties.deviceType));
    }

    free(devices);
}

U8 physicalDeviceHasExtensions(VkPhysicalDevice device) {
    U32 extensionsCount = 0;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionsCount, NULL);
    VkExtensionProperties* extensions = malloc(sizeof(VkExtensionProperties) * extensionsCount);
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionsCount, extensions);

    U32 extensionsPresent = 0;
    for (U32 i = 0; i < REQUIRED_EXTENSIONS_COUNT; i++) {
    for (U32 j = 0; j < extensionsCount; j++) {
        if (!strcmp(requiredExtensions[i], extensions[j].extensionName))
            extensionsPresent++;
    }}

    return extensionsPresent == REQUIRED_EXTENSIONS_COUNT;
}

VkPhysicalDevice physicalDeviceGetDevice(U8 desiredType) {
    U32 deviceCount = physicalDeviceCount();
    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);

    vkEnumeratePhysicalDevices(renderer(), &deviceCount, devices);

    for (U32 i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);

        if (!queueFamiliesHasType(devices[i], VK_QUEUE_GRAPHICS_BIT)) continue;
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(devices[i], i, windowSurfaceGet(), &presentSupport);
        if (!presentSupport) continue;
        if (!physicalDeviceHasExtensions(devices[i])) continue;

        if (deviceProperties.deviceType == desiredType) return devices[i];
    }

    return VK_NULL_HANDLE;
}

VkPhysicalDevice physicalDeviceInit(void) {
    VkPhysicalDevice device = VK_NULL_HANDLE;
    physicalDeviceList();

    VkPhysicalDeviceProperties deviceProperties;

    U32 i = 0;
    while (device == VK_NULL_HANDLE && i < PHYSICAL_DEVICE_TYPE_COUNT) {
        device = physicalDeviceGetDevice(devicePriorityList[i++]);
    }
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    logI("GPU's selected: %s (%s)", deviceProperties.deviceName, physicalDeviceTypeName(deviceProperties.deviceType));
    return device;
}
