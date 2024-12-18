#include "GLFW/glfw3.h"
#include <stdlib.h>

GLFWwindow* createWindow() {
    GLFWwindow* window;

    if (!glfwInit())
        exit(1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1280, 720, "Mainraft", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(2);
    }

    glfwMakeContextCurrent(window);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return window;
}

void clearWindow() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void updateWindow(GLFWwindow* window) {
    glfwSwapBuffers(window);
    glfwPollEvents();
}