#pragma once

#include <vulkan/vulkan.h>
#include "utils/types.h"

#include "renderer/pipeline.h"
#include "renderer/swapChain.h"
#include "devices/logicalDevice.h"

VkPipelineShaderStageCreateInfo pipelineCreateShader(VkShaderStageFlagBits stage, VkShaderModule shader);
VkPipelineDynamicStateCreateInfo pipelineCreateDynamicState(void);
VkPipelineVertexInputStateCreateInfo pipelineCreateVertexInput(void);
VkPipelineInputAssemblyStateCreateInfo pipelineCreateInputAssembly(void);
VkViewport pipelineCreateViewport(void);
VkRect2D pipelineCreateScissor(void);
VkPipelineViewportStateCreateInfo pipelineCreateViewportState(VkViewport viewport, VkRect2D scissor);
VkPipelineRasterizationStateCreateInfo pipelineCreateRasterizerState(void);
VkPipelineMultisampleStateCreateInfo pipelineCreateMultisamplingState(void);
VkPipelineColorBlendAttachmentState pipelineCreateColorBlendAttachmentState(void);
VkPipelineColorBlendStateCreateInfo pipelineCreateColorBlendState(VkPipelineColorBlendAttachmentState colorBlendAttachment);
void pipelineDeleteLayout(VkPipelineLayout layout);
VkPipelineLayout pipelineCreateLayout(void);
void pipelineLayoutClean(VkPipelineLayout pipelineLayout);
void pipelineClean(VkPipeline graphicsPipeline);
VkPipeline pipelineInit(VkPipelineLayout layout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkRenderPass renderPass);
