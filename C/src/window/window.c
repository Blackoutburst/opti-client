#include <stdlib.h>
#include "graphics/opengl.h"
#include "glfw/glfw3.h"
#include "utils/types.h"

GLFWwindow* windowCreate() {
    GLFWwindow* window;

    if (!glfwInit())
        exit(1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1280, 720, "Mainraft", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(2);
    }

    glfwMakeContextCurrent(window);

    #if defined(_WIN32) || defined(_WIN64)
        glewInit();
    #endif

    glfwSwapInterval(1);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return window;
}

void windowUpdate(GLFWwindow* window) {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void windowClear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
