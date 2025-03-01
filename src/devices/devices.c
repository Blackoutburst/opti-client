#include <stdlib.h>

#include "utils/logger.h"
#include "devices/devices.h"
#include "devices/logicalDevice.h"
#include "devices/physicalDevice.h"
#include "renderer/rendererInstance.h"

static DEVICE* primaryDevice = NULL;
static DEVICE* secondaryDevice = NULL;

DEVICE* devicesGetPrimary(void) {
    return primaryDevice;
}

DEVICE* devicesGetSecondary(void) {
    return secondaryDevice;
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

    if (primaryDevice != NULL) {
        free(primaryDevice);
    }

    if (secondaryDevice != NULL) {
        free(secondaryDevice);
    }
}

void devicesCreate(DEVICE* device, U8 primary) {
    device->logical = primary ? logicalDeviceGetPrimary() : logicalDeviceGetSecondary();
    device->physical = primary ? physicalDeviceGetPrimary() : physicalDeviceGetSecondary();
    device->properties = physicalDeviceGetProperties(device->physical);
    device->graphicQueue = logicalDeviceGetQueue(device->physical, device->logical, VK_QUEUE_GRAPHICS_BIT);
    device->presentQueue = logicalDeviceGetPresentationQueue(device->physical, device->logical);
}

void devicesInit(void) {
    physicalDeviceInit(rendererInstanceGetInstance());
    logicalDeviceInit();

    if (logicalDeviceGetPrimary() != VK_NULL_HANDLE) {
        primaryDevice = malloc(sizeof(DEVICE));
        devicesCreate(primaryDevice, 1);
    }

    if (logicalDeviceGetSecondary() != VK_NULL_HANDLE) {
        secondaryDevice = malloc(sizeof(DEVICE));
        devicesCreate(secondaryDevice, 0);
    }
}


