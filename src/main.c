#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdio.h>

#include "utils/logger.h"
#include "renderer/rendererInstance.h"

int main(void) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "VKMC", NULL, NULL);

    rendererInstanceInit();

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    rendererInstanceClean();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
