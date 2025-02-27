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
