#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

VkPhysicalDevice physicalDeviceGetPrimary(void);
VkPhysicalDevice physicalDeviceGetSecondary(void);
void physicalDeviceInit(VkInstance instance);
const I8* physicalDeviceTypeName(VkPhysicalDeviceType type);
U32 physicalDeviceCount(VkInstance instance);
void physicalDeviceList(VkInstance instance);
VkPhysicalDevice physicalDeviceGetDevice(VkInstance instance, U8 desiredType, U32 primaryId);
