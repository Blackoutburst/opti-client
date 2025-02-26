#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

VkDebugUtilsMessengerCreateInfoEXT* logCallBackGetInfo(void);
VkDebugUtilsMessengerEXT logCallBackGetLogger(void);
void logCallBackClean(VkInstance instance);
void logCallBackInit(void);
void logCallBackSet(VkInstance instance);
VKAPI_ATTR VkBool32 VKAPI_CALL logCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
