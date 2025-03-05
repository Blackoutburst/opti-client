#include "renderer/shader.h"
#include "utils/io.h"
#include "devices/logicalDevice.h"

void shaderClean(VkShaderModule shader) {
    vkDestroyShaderModule(logicalDeviceGet(), shader, NULL);
}

VkShaderModule shaderInit(const I8* filePath) {
    FILE* file = ioOpenFile(filePath);
    U32 codeSize = ioFileSize(file);
    U32* code = (U32*)ioReadFile(file);

    VkShaderModuleCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.codeSize = codeSize;
    createInfo.pCode = code;

    VkShaderModule shaderModule;
    vkCreateShaderModule(logicalDeviceGet(), &createInfo, NULL, &shaderModule);

    return shaderModule;
}
