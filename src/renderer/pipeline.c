#include "renderer/pipeline.h"
#include "renderer/swapchain.h"
#include "devices/devices.h"

VkPipelineShaderStageCreateInfo pipelineCreateShader(VkShaderStageFlagBits stage, VkShaderModule shader) {
    VkPipelineShaderStageCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.stage = stage;
    createInfo.module = shader;
    createInfo.pName = "main";
    createInfo.pSpecializationInfo = NULL;

    return createInfo;
}

VkPipelineDynamicStateCreateInfo pipelineCreateDynamicState(void) {
    VkDynamicState dynamicStates[2] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    }; 

    VkPipelineDynamicStateCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.dynamicStateCount = 2;
    createInfo.pDynamicStates = dynamicStates;

    return createInfo;
}

VkPipelineVertexInputStateCreateInfo pipelineCreateVertexInput(void) {
    VkPipelineVertexInputStateCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.vertexBindingDescriptionCount = 0;
    createInfo.pVertexBindingDescriptions = NULL;
    createInfo.vertexAttributeDescriptionCount = 0;
    createInfo.pVertexAttributeDescriptions = NULL;

    return createInfo;
}

VkPipelineInputAssemblyStateCreateInfo pipelineCreateInputAssembly(void) {
    VkPipelineInputAssemblyStateCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    createInfo.primitiveRestartEnable = VK_FALSE;

    return createInfo;
}

VkViewport pipelineCreateViewport(void) {
    VkExtent2D extend = swapChainExtend();

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (F32) extend.width;
    viewport.height = (F32) extend.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    return viewport;
}

VkRect2D pipelineCreateScissor(void) {
    VkExtent2D extend = swapChainExtend();

    VkRect2D scissor;
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent = extend;

    return scissor;
}

VkPipelineViewportStateCreateInfo pipelineCreateViewportState(VkViewport viewport, VkRect2D scissor) {
    VkPipelineViewportStateCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.viewportCount = 1;
    createInfo.pViewports = &viewport;
    createInfo.scissorCount = 1;
    createInfo.pScissors = &scissor;

    return createInfo;
}

VkPipelineRasterizationStateCreateInfo pipelineCreateRasterizerState(void) {
    VkPipelineRasterizationStateCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.depthClampEnable = VK_FALSE;
    createInfo.rasterizerDiscardEnable = VK_FALSE;
    createInfo.polygonMode = VK_POLYGON_MODE_FILL;
    createInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    createInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    createInfo.depthBiasEnable = VK_FALSE;
    createInfo.depthBiasConstantFactor = 0.0f;
    createInfo.depthBiasClamp = 0.0f;
    createInfo.depthBiasSlopeFactor = 0.0f;
    createInfo.lineWidth = 1.0f;

    return createInfo;
}

VkPipelineMultisampleStateCreateInfo pipelineCreateMultisamplingState(void) {
    VkPipelineMultisampleStateCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.sampleShadingEnable = VK_FALSE;
    createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.minSampleShading = 1.0f;
    createInfo.pSampleMask = NULL;
    createInfo.alphaToCoverageEnable = VK_FALSE;
    createInfo.alphaToOneEnable = VK_FALSE;

    return createInfo;
}

VkPipelineColorBlendAttachmentState pipelineCreateColorBlendAttachmentState(void) {
    VkPipelineColorBlendAttachmentState createInfo;
    createInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    createInfo.blendEnable = VK_FALSE;
    createInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    createInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    createInfo.colorBlendOp = VK_BLEND_OP_ADD;
    createInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    createInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    createInfo.alphaBlendOp = VK_BLEND_OP_ADD;

    return createInfo;
}

VkPipelineColorBlendStateCreateInfo pipelineCreateColorBlendState(VkPipelineColorBlendAttachmentState colorBlendAttachment) {
    VkPipelineColorBlendStateCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.logicOpEnable = VK_FALSE;
    createInfo.logicOp = VK_LOGIC_OP_COPY;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &colorBlendAttachment;
    createInfo.blendConstants[0] = 0.0f;
    createInfo.blendConstants[1] = 0.0f;
    createInfo.blendConstants[2] = 0.0f;
    createInfo.blendConstants[3] = 0.0f;

    return createInfo;
}

void pipelineDeleteLayout(VkPipelineLayout layout) {
    vkDestroyPipelineLayout(devices()->logical, layout, NULL);
}

VkPipelineLayout pipelineCreateLayout(void) {
    VkPipelineLayout pipelineLayout;
    
    VkPipelineLayoutCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.setLayoutCount = 0;
    createInfo.pSetLayouts = NULL;
    createInfo.pushConstantRangeCount = 0;
    createInfo.pPushConstantRanges = NULL;
    
    vkCreatePipelineLayout(devices()->logical, &createInfo, NULL, &pipelineLayout);

    return pipelineLayout;
}

void pipelineLayoutClean(VkPipelineLayout pipelineLayout) {
    vkDestroyPipelineLayout(devices()->logical, pipelineLayout, NULL);
}

void pipelineClean(VkPipeline graphicsPipeline) {
    vkDestroyPipeline(devices()->logical, graphicsPipeline, NULL);
}

VkPipeline pipelineInit(VkPipelineLayout layout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkRenderPass renderPass) {
    UNUSED_VAR(layout);
    
    VkPipelineShaderStageCreateInfo vertexInfo = pipelineCreateShader(VK_SHADER_STAGE_VERTEX_BIT, vertexShader);
    VkPipelineShaderStageCreateInfo fragmentInfo = pipelineCreateShader(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader);
    VkPipelineShaderStageCreateInfo shaderInfo[2] = {vertexInfo, fragmentInfo};

    VkPipelineVertexInputStateCreateInfo vertexInput = pipelineCreateVertexInput();
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = pipelineCreateInputAssembly();
    VkPipelineViewportStateCreateInfo viewport = pipelineCreateViewportState(pipelineCreateViewport(), pipelineCreateScissor());
    VkPipelineRasterizationStateCreateInfo rasterizer = pipelineCreateRasterizerState();
    VkPipelineMultisampleStateCreateInfo multiSampling = pipelineCreateMultisamplingState();
    VkPipelineColorBlendStateCreateInfo colorBlend =  pipelineCreateColorBlendState(pipelineCreateColorBlendAttachmentState());
    VkPipelineDynamicStateCreateInfo dynamicState = pipelineCreateDynamicState();

    VkGraphicsPipelineCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.pNext = NULL;
    createInfo.stageCount = 2;
    createInfo.pStages = shaderInfo;
    createInfo.pVertexInputState = &vertexInput;
    createInfo.pInputAssemblyState = &inputAssembly;
    createInfo.pTessellationState = NULL;
    createInfo.pViewportState = &viewport;
    createInfo.pRasterizationState = &rasterizer;
    createInfo.pMultisampleState = &multiSampling;
    createInfo.pDepthStencilState = NULL;
    createInfo.pColorBlendState = &colorBlend;
    createInfo.pDynamicState = &dynamicState;
    createInfo.layout = layout;
    createInfo.renderPass = renderPass;
    createInfo.subpass = 0;
    createInfo.basePipelineHandle = VK_NULL_HANDLE;
    createInfo.basePipelineIndex = -1;

    VkPipeline graphicsPipeline;
    vkCreateGraphicsPipelines(devices()->logical, VK_NULL_HANDLE, 1, &createInfo, NULL, &graphicsPipeline);

    return graphicsPipeline;

}
