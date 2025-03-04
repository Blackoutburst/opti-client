#pragma once

#include <vulkan/vulkan.h>

#include "utils/types.h"

VkImageView* imageViewGet(void);
void imageViewInit(void);
void imageViewClean(void);
