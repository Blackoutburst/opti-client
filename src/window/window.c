#include "window/window.h"

static GLFWwindow* window;

GLFWwindow* windowGetHandle(void) {
    return window;
}

void windowUpdate(void) {
    glfwPollEvents();
}

void windowClear(void) {

}

void windowClean(void) {
    glfwDestroyWindow(window);

    glfwTerminate();
}

void windowInit(void) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(800, 600, "VKMC", NULL, NULL);
}

