#pragma once

#include <vulkan/vulkan.h>
#include <stdlib.h>

#include "utils/types.h"

U32 queueFamiliesCount(VkPhysicalDevice device);
U8 queueFamiliesFindType(VkPhysicalDevice device, VkQueueFlagBits type);
