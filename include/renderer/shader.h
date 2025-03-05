#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

void shaderClean(VkShaderModule shader);
VkShaderModule shaderInit(const I8* filePath);
