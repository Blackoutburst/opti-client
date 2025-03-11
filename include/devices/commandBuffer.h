#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

void commandBufferBegin(VkCommandBuffer cmdBuffer);
void commandBufferEnd(VkCommandBuffer cmdBuffer);
void commandBufferClean(VkCommandBuffer cmdBuffer, VkCommandPool cmdPool);
VkCommandBuffer commandBufferInit(VkCommandPool cmdPool);
