#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

typedef struct device DEVICE;

struct device {
    VkDevice logical;
    VkPhysicalDevice physical;
    VkPhysicalDeviceProperties properties;
    VkQueue graphicsQueue;
};

DEVICE* devicesGetPrimary(void);
DEVICE* devicesGetSecondary(void);
void devicesCreate(DEVICE* device, U8 primary);
void devicesPrint(DEVICE* device);
void devicesClean(void);
void devicesInit(void);

