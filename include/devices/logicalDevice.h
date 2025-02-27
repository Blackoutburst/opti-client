#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

VkDevice logicalDeviceGetPrimary(void);
VkDevice logicalDeviceGetSecondary(void);
void logicalDeviceClean(void);
void logicalDeviceCreate(VkPhysicalDevice physicalDevice, VkDevice* logicalDevice);
void logicalDeviceInit(void);

