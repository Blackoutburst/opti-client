#include "renderer/windowSurface.h"

static VkSurfaceKHR surface;

VkSurfaceKHR windowSurfaceGet(void) {
    return surface;
}

void windowSurfaceClean(VkInstance instance) {
    vkDestroySurfaceKHR(instance, surface, NULL);
}

void windowSurfaceInit(VkInstance instance, GLFWwindow* window) {
    glfwCreateWindowSurface(instance, window, NULL, &surface);
}
