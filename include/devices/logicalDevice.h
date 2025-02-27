#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

VkDevice logicalDeviceGetPrimary(void);
VkDevice logicalDeviceGetSecondary(void);
VkQueue logicalDeviceGetQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkQueueFlagBits queueType);
void logicalDeviceClean(void);
void logicalDeviceCreate(VkPhysicalDevice physicalDevice, VkDevice* logicalDevice);
void logicalDeviceInit(void);

