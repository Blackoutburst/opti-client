#pragma once

#include <GLFW/glfw3.h>
#include "utils/types.h"

GLFWwindow* windowGetHandle(void);
void windowUpdate(void);
void windowClear(void);
void windowClean(void);
void windowInit(void);
