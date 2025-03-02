#include <stdlib.h>
#include <string.h>

#include "utils/logger.h"
#include "devices/physicalDevice.h"
#include "devices/queueFamilies.h"
#include "renderer/windowSurface.h"

#define PHYSICAL_DEVICE_TYPE_COUNT 5

static VkPhysicalDevice device = VK_NULL_HANDLE;

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

VkPhysicalDevice physicalDeviceGet(void) {
    return device;
}

VkPhysicalDeviceProperties physicalDeviceGetProperties(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    return properties;
}

void physicalDeviceInit(VkInstance instance) {
    physicalDeviceList(instance);

    VkPhysicalDeviceProperties deviceProperties;

    U32 i = 0;
    while (device == VK_NULL_HANDLE && i < PHYSICAL_DEVICE_TYPE_COUNT) {
        device = physicalDeviceGetDevice(instance, devicePriorityList[i++], 0);
    }
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    logI("GPU's selected: %s (%s)", deviceProperties.deviceName, physicalDeviceTypeName(deviceProperties.deviceType));
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

VkPhysicalDevice physicalDeviceGetDevice(VkInstance instance, U8 desiredType, U32 primaryId) {
    U32 deviceCount = physicalDeviceCount(instance);
    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);

    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    for (U32 i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);

        if (deviceProperties.deviceID == primaryId) continue;
        if (!queueFamiliesHasType(devices[i], VK_QUEUE_GRAPHICS_BIT)) continue;
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(devices[i], i, windowSurfaceGet(), &presentSupport);
        if (!presentSupport) continue;
        if (!physicalDeviceHasExtensions(devices[i])) continue;

        if (deviceProperties.deviceType == desiredType) return devices[i];
    }

    return VK_NULL_HANDLE;
}
