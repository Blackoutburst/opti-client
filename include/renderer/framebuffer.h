#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

void framebufferClean(VkSwapchainKHR swapchain, VkFramebuffer* framebuffer);
VkFramebuffer* framebufferInit(VkSwapchainKHR swapchain, VkRenderPass renderPass, VkImageView* imageView);
