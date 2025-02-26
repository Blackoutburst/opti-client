#include <stdlib.h>
#include <stdio.h>

#include "debug/logCallback.h"
#include "utils/logger.h"

static VkDebugUtilsMessengerEXT logger;
static VkDebugUtilsMessengerCreateInfoEXT* createInfo;


VkDebugUtilsMessengerCreateInfoEXT* logCallBackGetInfo(void) {
    return createInfo;
}

VkDebugUtilsMessengerEXT logCallBackGetLogger(void) {
    return logger;
}

void logCallBackClean(VkInstance instance) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    
    if (func) {
        func(instance, logger, NULL);
        logI("Logger deleted successfully");
    } else {
        logW("Logger deletion failed");
    }

    free(createInfo);
}

void logCallBackInit(void) {
    createInfo = malloc(sizeof(VkDebugUtilsMessengerCreateInfoEXT));
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo->pNext = NULL;
    createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
    createInfo->pfnUserCallback = logCallback;
    createInfo->pUserData = NULL;
}

void logCallBackSet(VkInstance instance) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    
    if (func) {
        func(instance, createInfo, NULL, &logger);
        logI("Logger set successfully");
    } else {
        logW("Logger creation failed");
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL logCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    UNUSED_VAR(messageType);
    UNUSED_VAR(pUserData);
    
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: logT("%s", pCallbackData->pMessage); break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: logI("%s", pCallbackData->pMessage); break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: logW("%s", pCallbackData->pMessage); break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: logE("%s", pCallbackData->pMessage); break;
        default: logMsg("%s", pCallbackData->pMessage);
    }
    
    return VK_FALSE;
}
