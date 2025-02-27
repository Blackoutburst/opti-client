#include <stdlib.h>

#include "renderer/queueFamilies.h"

U32 queueFamiliesCount(VkPhysicalDevice device) {
    U32 count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, NULL);

    return count;
}


U8 queueFamiliesFindType(VkPhysicalDevice device, VkQueueFlagBits type) {
    U32 queueCount = queueFamiliesCount(device);
    VkQueueFamilyProperties* queues = malloc(sizeof(VkQueueFamilyProperties) * queueCount);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queues);

    for (U32 i = 0; i < queueCount; i++) {
        if (queues[i].queueFlags & type) return 1;
    }

    return 0;
}
