#pragma once
#include "world/chunk.h"
#include "utils/types.h"

#define WG_QUEUE_SIZE 4096
#define WG_VAO_QUEUE_SIZE 4096

typedef struct worldGenQueue WG_QUEUE;
typedef struct worldGenQueueElement WG_QUEUE_ELEM;

struct worldGenQueueElement {
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

    DWORD WINAPI wgThread4WIN32(LPVOID arg);
    DWORD WINAPI wgThread3WIN32(LPVOID arg);
    DWORD WINAPI wgThread2WIN32(LPVOID arg);
    DWORD WINAPI wgThread1WIN32(LPVOID arg);
    void wgInitThreadPoolWIN32();
#else
    #include <pthread.h>

    void* wgThread4POSIX(void* arg);
    void* wgThread3POSIX(void* arg);
    void* wgThread2POSIX(void* arg);
    void* wgThread1POSIX(void* arg);
    void wgInitThreadPoolPOSIX();
#endif

void _wgVAOQueueCleanElement(WG_VAO_QUEUE* queue, U16 index);
void wgVAOQueue1CleanElement(U16 index);
void wgVAOQueue2CleanElement(U16 index);
void wgVAOQueue3CleanElement(U16 index);
void wgVAOQueue4CleanElement(U16 index);
void wgVAOQueuePush(WG_VAO_QUEUE* queue, CHUNK* chunk, I32* mesh);
U8 _wgVAOQueuePop(WG_VAO_QUEUE* queue, WG_VAO_QUEUE_ELEM** element);
U8 wgVAOQueue1Pop(WG_VAO_QUEUE_ELEM** element);
U8 wgVAOQueue2Pop(WG_VAO_QUEUE_ELEM** element);
U8 wgVAOQueue3Pop(WG_VAO_QUEUE_ELEM** element);
U8 wgVAOQueue4Pop(WG_VAO_QUEUE_ELEM** element);
void wgFreeVAOQueue(WG_VAO_QUEUE* queue);
void wgInitVAOQueue(WG_VAO_QUEUE** queue);

void wgQueueCleanElement(WG_QUEUE* queue, U16 index);
void _wgQueuePush(WG_QUEUE* queue, CHUNK* chunk);
void wgQueue1Push(CHUNK* chunk);
void wgQueue2Push(CHUNK* chunk);
void wgQueue3Push(CHUNK* chunk);
void wgQueue4Push(CHUNK* chunk);
void wgQueuePush(CHUNK* chunk);
U8 _wgQueuePop(WG_QUEUE* queue, WG_QUEUE_ELEM** element);
U8 wgQueue1Pop(WG_QUEUE_ELEM** element);
U8 wgQueue2Pop(WG_QUEUE_ELEM** element);
U8 wgQueue3Pop(WG_QUEUE_ELEM** element);
U8 wgQueue4Pop(WG_QUEUE_ELEM** element);
void wgFreeQueue(WG_QUEUE* queue);
void wgInitQueue(WG_QUEUE** queue);

void wgCleanThreadPool();
void wgInitThreadPool();
