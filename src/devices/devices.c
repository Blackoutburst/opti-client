#include <stdlib.h>

#include "utils/logger.h"
#include "devices/devices.h"
#include "devices/logicalDevice.h"
#include "devices/physicalDevice.h"
#include "renderer/rendererInstance.h"

static DEVICE* device = NULL;

DEVICE* devicesGet(void) {
    return device;
}

void devicesPrint(DEVICE* device) {
    if (device == NULL) {
        logI("Device: NULL");
    } else {
        logI("Device: %s (%s)", device->properties.deviceName, physicalDeviceTypeName(device->properties.deviceType));
    }
}

void devicesClean(void) {
    logicalDeviceClean();

    if (device != NULL) {
        free(device);
    }
}

void devicesCreate(DEVICE* device) {
    device->logical = logicalDeviceGet();
    device->physical = physicalDeviceGet();
    device->properties = physicalDeviceGetProperties(device->physical);
    device->graphicQueue = logicalDeviceGetQueue(device->physical, device->logical, VK_QUEUE_GRAPHICS_BIT);
    device->presentQueue = logicalDeviceGetPresentationQueue(device->physical, device->logical);
}

void devicesInit(void) {
    physicalDeviceInit(rendererInstanceGetInstance());
    logicalDeviceInit();

    if (logicalDeviceGet() != VK_NULL_HANDLE) {
        device = malloc(sizeof(DEVICE));
        devicesCreate(device);
    }
}


