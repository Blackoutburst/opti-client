#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

void pipelineClean(void);
void pipelineInit(VkShaderModule vertexShader, VkShaderModule fragmentShader);
