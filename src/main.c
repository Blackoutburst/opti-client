#define GLFW_INCLUDE_VULKAN
#define LOGGER_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdio.h>

#include "utils/logger.h"
#include "utils/types.h"
#include "utils/args.h"
#include "renderer/rendererInstance.h"
#include "window/window.h"

I32 main(I32 argc, I8** argv) {
    argsParse(argc, argv);
    windowInit();

    rendererInstanceInit();

    logD("test %i ss %s", 4, "test");

    while(!glfwWindowShouldClose(windowGetHandle())) {
        windowUpdate();

        windowClear();
    }

    rendererInstanceClean();

    windowClean();

    return 0;
}
