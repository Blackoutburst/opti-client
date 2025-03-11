#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

VkQueue logicalDeviceGetQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkQueueFlagBits queueType);
VkQueue logicalDeviceGetPresentationQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice);
U32 logicalDeviceGetSurfaceSupport(VkPhysicalDevice device);
void logicalDeviceClean(VkDevice device);
VkDevice logicalDeviceCreate(VkPhysicalDevice physicalDevice);
VkDevice logicalDeviceInit(VkPhysicalDevice physicalDevice);

