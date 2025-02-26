#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>

#include "utils/args.h"
#include "debug/validationLayers.h"
#include "renderer/rendererInstance.h"
#include "utils/logger.h"
#include "utils/vkerror.h"

static VkInstance instance;

VkInstance rendererInstanceGetInstance(void) {
    return instance;
}

void rendererInstanceClean(void) {
    vkDestroyInstance(instance, NULL);
}

void rendererInstanceInit(void) {
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "VKMC";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    U32 glfwExtensionCount = 0;
    const I8** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    const I8** requiredExtensions = malloc((glfwExtensionCount + 1) * sizeof(char*));
    memcpy(requiredExtensions, glfwExtensions, glfwExtensionCount * sizeof(char*));
    requiredExtensions[glfwExtensionCount] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
    glfwExtensionCount++;

    VkInstanceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    createInfo.pApplicationInfo = &appInfo;

    if (argsGetValidationLayers()) {
        createInfo.enabledLayerCount = validationLayersCount();
        createInfo.ppEnabledLayerNames = validationLayersGet();
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;
    }

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
    if (result != VK_SUCCESS) {
        logE("VK instance creation failed code: %s", vkError(result));
        exit(1);
    }

    logI("VK Instance created successfully");

    if (argsGetValidationLayers()) {
        logI("Enabled %i validation layers", validationLayersCount());
    }
}
