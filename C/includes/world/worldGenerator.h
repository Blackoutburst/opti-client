#pragma once
#include "world/chunk.h"
#include "utils/types.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
#else
    #include <unistd.h>
    #include <pthread.h>
#endif

#define WG_QUEUE_SIZE 4096 * 2
#define WG_VAO_QUEUE_SIZE 4096
#define WG_THREAD_COUNT 4

typedef struct worldGenQueue WG_QUEUE;
typedef struct worldGenQueueElement WG_QUEUE_ELEM;

struct worldGenQueueElement {
    U8 used;
    U16 id;
    CHUNK* chunk;
};

struct worldGenQueue {
    U16 size;
    U16 pushIndex;
    U16 popIndex;
    WG_QUEUE_ELEM** elements;
};


typedef struct worldGenVAOQueue WG_VAO_QUEUE;
typedef struct worldGenVAOQueueElement WG_VAO_QUEUE_ELEM;

struct worldGenVAOQueueElement {
    U8 used;
    U16 id;
    CHUNK* chunk;
    I32* mesh;
};

struct worldGenVAOQueue {
    U16 size;
    U16 pushIndex;
    U16 popIndex;
    WG_VAO_QUEUE_ELEM** elements;
};

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>

    DWORD WINAPI wgThreadWIN32(LPVOID arg);
    void wgInitThreadPoolWIN32();
#else
    #include <pthread.h>

    void* wgThreadPOSIX(void* arg);
    void wgInitThreadPoolPOSIX();
#endif

void wgVAOQueueCleanElement(U16 index);
void wgVAOQueuePush(CHUNK* chunk, I32* mesh);
U8 wgVAOQueuePop(WG_VAO_QUEUE_ELEM** element);
void wgFreeVAOQueue();
void wgInitVAOQueue();

void wgQueueCleanElement(U16 index);
void wgQueuePush(CHUNK* chunk);
U8 wgQueuePop(WG_QUEUE_ELEM** element);
void wgFreeQueue();
void wgInitQueue();

void wgCleanThreadPool();
void wgInitThreadPool();
