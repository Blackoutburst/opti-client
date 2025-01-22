#pragma once

#include "utils/types.h"
#include "world/meshQueue.h"
#include "world/chunk.h"

#define WG_THREAD_COUNT 4

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    DWORD WINAPI wgThreadWIN32(LPVOID arg);
    void wgInitThreadPoolWIN32();
#else
    void* wgThreadPOSIX(void* arg);
    void wgInitThreadPoolPOSIX();
#endif

void wgThreadMethod(MESH_QUEUE_ELEM* element);
void wgCleanThreadPool();
void wgInitThreadPool();
