#include <stdio.h>

#include "utils/types.h"
#include "utils/mutex.h"
#include "world/chunk.h"
#include "world/meshQueue.h"
#include "world/vaoQueue.h"
#include "world/worldGenerator.h"

static U8 running = 0;

void wgThreadMethod(MESH_QUEUE_ELEM* element) {
    I32* mesh = chunkGenerateMesh(element->chunk);
    vaoQueuePush(element->chunk, mesh);
    meshQueueCleanElement(element->id);
}

#if defined(_WIN32) || defined(_WIN64)
    DWORD WINAPI wgThreadWIN32(LPVOID arg) {
        MESH_QUEUE_ELEM* element = NULL;
        while (running) {
            while(meshQueuePop(&element)) {
                wgThreadMethod(element);
            }
            Sleep(1);
        }

        return 0;
    }
#else
    void* wgThreadPOSIX(void* arg) {
        MESH_QUEUE_ELEM* element = NULL;
        while (running) {
            while(meshQueuePop(&element)) {
                wgThreadMethod(element);
            }
            usleep(1000);
        }
        
        return NULL;
    }
#endif

#if defined(_WIN32) || defined(_WIN64)
    void wgInitThreadPoolWIN32() {
        for (U8 i = 0; i < WG_THREAD_COUNT; i++) {
            HANDLE thread = CreateThread(NULL, 0, wgThreadWIN32, NULL, 0, NULL);
            if (thread == NULL) {
                printf("World gen thread %i creation failed\n", i);
                return;
            }
            CloseHandle(thread);
        }
    }
#else
    void wgInitThreadPoolPOSIX() {
        for (U8 i = 0; i < WG_THREAD_COUNT; i++) {
            pthread_t thread;
            if (pthread_create(&thread, NULL, wgThreadPOSIX, NULL)) {
                printf("World gen thread %i creation failed\n", i);
                return;
            }
        }
    }
#endif

void wgCleanThreadPool() {
    running = 0;

    meshQueueFree();
    vaoQueueFree();
}

void wgInitThreadPool() {
    meshQueueInit();
    vaoQueueInit();

    running = 1;
    #if defined(_WIN32) || defined(_WIN64)
        wgInitThreadPoolWIN32();
    #else
        wgInitThreadPoolPOSIX();
    #endif
}
