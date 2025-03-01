#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "utils/types.h"

VkSurfaceKHR windowSurfaceGet(void);
void windowSurfaceClean(VkInstance instance);
void windowSurfaceInit(VkInstance instance, GLFWwindow* window);
