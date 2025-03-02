#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

typedef struct device DEVICE;

struct device {
    VkDevice logical;
    VkPhysicalDevice physical;
    VkPhysicalDeviceProperties properties;
    VkQueue graphicQueue;
    VkQueue presentQueue;
};

DEVICE* devicesGet(void);
void devicesCreate(DEVICE* device);
void devicesPrint(DEVICE* device);
void devicesClean(void);
void devicesInit(void);

