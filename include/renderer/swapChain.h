#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

VkImage* swapChainGetImages(VkSwapchainKHR swapchain);
U32 swapChainGetImagesCount(VkSwapchainKHR swapchain);
VkSurfaceFormatKHR swapChainFormat(void);
VkPresentModeKHR swapChainPresentMode(void);
VkSurfaceCapabilitiesKHR swapChainCapabilities(void);
VkExtent2D swapChainExtend(void);
void swapChainClean(VkSwapchainKHR swapchain);
VkSwapchainKHR swapChainInit(void);
