#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

VkImage* swapChainGetImages(void);
VkSwapchainKHR swapChainGet(void);
VkSurfaceFormatKHR swapChainFormat(void);
VkPresentModeKHR swapChainPresentMode(void);
VkExtent2D swapChainExtend(void);
VkSurfaceCapabilitiesKHR swapChainCapabilities(void);
void swapChainClean(void);
void swapChainInit(void);
