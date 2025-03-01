#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

VkDevice logicalDeviceGetPrimary(void);
VkDevice logicalDeviceGetSecondary(void);
VkQueue logicalDeviceGetQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkQueueFlagBits queueType);
VkQueue logicalDeviceGetPresentationQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice);
U32 logicalDeviceGetSurfaceSupport(VkPhysicalDevice device);
void logicalDeviceClean(void);
void logicalDeviceCreate(VkPhysicalDevice physicalDevice, VkDevice* logicalDevice);
void logicalDeviceInit(void);

