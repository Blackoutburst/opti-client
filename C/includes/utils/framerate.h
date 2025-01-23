#pragma once

#include <stdio.h>
#include "utils/types.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <time.h>
#endif

void FPS();
