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

DEVICE* devices(void);
void devicesCreate(VkPhysicalDevice physical, VkDevice logical);
void devicesPrint(void);
void devicesClean(void);
void devicesInit(VkPhysicalDevice physical, VkDevice logical);

