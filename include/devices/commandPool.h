#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

void commandPoolClean(VkCommandPool cmdPool);
VkCommandPool commandPoolInit(void);
