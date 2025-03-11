#include "devices/commandBuffer.h"
#include "devices/devices.h"

void commandBufferEnd(VkCommandBuffer cmdBuffer) {
    vkEndCommandBuffer(cmdBuffer);
}

void commandBufferBegin(VkCommandBuffer cmdBuffer) {
    VkCommandBufferBeginInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.pInheritanceInfo = NULL;

    vkBeginCommandBuffer(cmdBuffer, &createInfo);
}

void commandBufferClean(VkCommandBuffer cmdBuffer, VkCommandPool cmdPool) {
    vkFreeCommandBuffers(devices()->logical, cmdPool, 1, &cmdBuffer);
}

VkCommandBuffer commandBufferInit(VkCommandPool cmdPool) {
    VkCommandBuffer cmdBuffer;

    VkCommandBufferAllocateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    createInfo.pNext = NULL;
    createInfo.commandPool = cmdPool;
    createInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    createInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(devices()->logical, &createInfo, &cmdBuffer);
    
    return cmdBuffer;
}

