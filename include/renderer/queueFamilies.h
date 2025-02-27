#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

U32 queueFamiliesCount(VkPhysicalDevice device);
U8 queueFamiliesHasType(VkPhysicalDevice device, VkQueueFlagBits type);
U32 queueFamiliesGetType(VkPhysicalDevice device, VkQueueFlagBits type);
