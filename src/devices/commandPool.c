#include "devices/commandPool.h"
#include "devices/devices.h"
#include "devices/queueFamilies.h"

void commandPoolClean(VkCommandPool cmdPool) {
    vkDestroyCommandPool(devices()->logical, cmdPool, NULL);
}

VkCommandPool commandPoolInit(void) {
    VkCommandPool cmdPool;
    U32 graphicQueue = queueFamiliesGetType(devices()->physical, VK_QUEUE_GRAPHICS_BIT);

    VkCommandPoolCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    createInfo.queueFamilyIndex = graphicQueue;

    vkCreateCommandPool(devices()->logical, &createInfo, NULL, &cmdPool);

    return cmdPool;
}

