#include <stdlib.h>

#include "utils/logger.h"
#include "devices/devices.h"
#include "devices/logicalDevice.h"
#include "devices/physicalDevice.h"
#include "renderer/rendererInstance.h"

static DEVICE* device = NULL;

DEVICE* devices(void) {
    return device;
}

void devicesPrint(void) {
    if (device == NULL) {
        logI("Device: NULL");
    } else {
        logI("Device: %s (%s)", device->properties.deviceName, physicalDeviceTypeName(device->properties.deviceType));
    }
}

void devicesClean(void) {
    logicalDeviceClean(device->logical);

    if (device != NULL) {
        free(device);
    }
}

void devicesCreate(VkPhysicalDevice physical, VkDevice logical) {
    device->physical = physical;
    device->logical = logical;
    device->properties = physicalDeviceGetProperties(device->physical);
    device->graphicQueue = logicalDeviceGetQueue(device->physical, device->logical, VK_QUEUE_GRAPHICS_BIT);
    device->presentQueue = logicalDeviceGetPresentationQueue(device->physical, device->logical);
}

void devicesInit(VkPhysicalDevice physical, VkDevice logical) {
    if (physical != VK_NULL_HANDLE && logical != VK_NULL_HANDLE) {
        device = malloc(sizeof(DEVICE));
        devicesCreate(physical, logical);
    }
}


