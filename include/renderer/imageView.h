#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

void imageViewClean(VkImageView* imageView, VkSwapchainKHR swapchain);
VkImageView* imageViewInit(VkSwapchainKHR swapchain);

