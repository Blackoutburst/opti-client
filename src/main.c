#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdio.h>

#include "utils/logger.h"
#include "utils/types.h"
#include "renderer/rendererInstance.h"
#include "window/window.h"

I32 main(void) {
    windowInit();

    rendererInstanceInit();

    while(!glfwWindowShouldClose(windowGetHandle())) {
        windowUpdate();

        windowClear();
    }

    rendererInstanceClean();

    windowClean();

    return 0;
}
