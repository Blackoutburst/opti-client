#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

void renderPassEnd(VkCommandBuffer cmdBuffer);
void renderPassBegin(VkRenderPass renderPass, VkFramebuffer* framebuffers, VkCommandBuffer cmdBuffer, U32 imageIndex);
void renderPassClean(VkRenderPass renderPass);
VkRenderPass renderPassInit(void);

