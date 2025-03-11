#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

VkPhysicalDeviceProperties physicalDeviceGetProperties(VkPhysicalDevice device);
const I8* physicalDeviceTypeName(VkPhysicalDeviceType type);
U32 physicalDeviceCount(void);
void physicalDeviceList(void);
VkPhysicalDevice physicalDeviceGetDevice(U8 desiredType);
VkPhysicalDevice physicalDeviceInit(void);

