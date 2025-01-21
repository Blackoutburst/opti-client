#include <stdlib.h>
#include "utils/ioUtils.h"
#include "utils/math.h"
#include "utils/types.h"
#include "world/worldGenerator.h"
#include "world/world.h"

static U8 running = 0;

static WG_QUEUE* queue = NULL;
static WG_VAO_QUEUE* vaoQueue = NULL;

#if defined(_WIN32) || defined(_WIN64)
    CRITICAL_SECTION wgVAOQueueMutex;
#else
    pthread_mutex_t wgVAOQueueMutex;
#endif

#if defined(_WIN32) || defined(_WIN64)
    CRITICAL_SECTION wgQueueMutex;
#else
    pthread_mutex_t wgQueueMutex;
#endif

void wgVAOQueueCleanElement(U16 index) {
    if (vaoQueue == NULL) return;
    WG_VAO_QUEUE_ELEM* element = vaoQueue->elements[index];
    if (element == NULL) return;
    vaoQueue->elements[index]->used = 0;
    vaoQueue->elements[index]->chunk = NULL;
    vaoQueue->elements[index]->mesh = NULL;
}

void wgVAOQueuePush(CHUNK* chunk, I32* mesh) {
    if (vaoQueue == NULL) return;

    #if defined(_WIN32) || defined(_WIN64)
        EnterCriticalSection(&wgVAOQueueMutex);
    #else
        pthread_mutex_lock(&wgVAOQueueMutex);
    #endif
    
    if (vaoQueue->elements[vaoQueue->pushIndex]->used) {
        wgVAOQueueCleanElement(vaoQueue->pushIndex);
    }
    vaoQueue->elements[vaoQueue->pushIndex]->chunk = chunk;
    vaoQueue->elements[vaoQueue->pushIndex]->mesh = mesh;
    vaoQueue->elements[vaoQueue->pushIndex]->id = vaoQueue->pushIndex;
    vaoQueue->elements[vaoQueue->pushIndex]->used = 1;

    vaoQueue->pushIndex++;
    if (vaoQueue->pushIndex >= vaoQueue->size) {
        vaoQueue->pushIndex = 0;
    }

    #if defined(_WIN32) || defined(_WIN64)
        LeaveCriticalSection(&wgVAOQueueMutex);
    #else
        pthread_mutex_unlock(&wgVAOQueueMutex);
    #endif
}

U8 wgVAOQueuePop(WG_VAO_QUEUE_ELEM** element) {
    if (vaoQueue == NULL) return 0;

    #if defined(_WIN32) || defined(_WIN64)
        EnterCriticalSection(&wgVAOQueueMutex);
    #else
        pthread_mutex_lock(&wgVAOQueueMutex);
    #endif
    
    WG_VAO_QUEUE_ELEM* elem = vaoQueue->elements[vaoQueue->popIndex];
    *element = elem;
    if (elem == NULL || !elem->used) {
        #if defined(_WIN32) || defined(_WIN64)
            LeaveCriticalSection(&wgVAOQueueMutex);
        #else
            pthread_mutex_unlock(&wgVAOQueueMutex);
        #endif
        return 0;
    }

    vaoQueue->popIndex++;
    if (vaoQueue->popIndex >= vaoQueue->size) {
        vaoQueue->popIndex = 0;
    }

    #if defined(_WIN32) || defined(_WIN64)
        LeaveCriticalSection(&wgVAOQueueMutex);
    #else
        pthread_mutex_unlock(&wgVAOQueueMutex);
    #endif

    return 1;
}

void wgFreeVAOQueue() {
    for (int i = 0; i < WG_VAO_QUEUE_SIZE; i++) {
        wgVAOQueueCleanElement(i);
        free(vaoQueue->elements[i]);
    }
    free(vaoQueue->elements);
    free(vaoQueue);
    vaoQueue = NULL;

    #if defined(_WIN32) || defined(_WIN64)
        DeleteCriticalSection(&wgVAOQueueMutex);
    #else
        pthread_mutex_destroy(&wgVAOQueueMutex);
    #endif
}

void wgInitVAOQueue() {
    WG_VAO_QUEUE* q = malloc(sizeof(WG_VAO_QUEUE));
    q->size = WG_VAO_QUEUE_SIZE;
    q->pushIndex = 0;
    q->popIndex = 0;
    q->elements = malloc(sizeof(WG_VAO_QUEUE_ELEM*) * WG_VAO_QUEUE_SIZE);
    for (int i = 0; i < WG_VAO_QUEUE_SIZE; i++) {
        q->elements[i] = malloc(sizeof(WG_VAO_QUEUE_ELEM));
        q->elements[i]->chunk = NULL;
        q->elements[i]->mesh = NULL;
        q->elements[i]->id = 0;
        q->elements[i]->used = 0;
    }
    vaoQueue = q;

    #if defined(_WIN32) || defined(_WIN64)
        InitializeCriticalSection(&wgVAOQueueMutex);
    #else
        pthread_mutex_init(&wgVAOQueueMutex, NULL);
    #endif
}

void wgThreadMethod(WG_QUEUE_ELEM* element) {
    I32* mesh = generateChunkMesh(element->chunk);
    wgVAOQueuePush(element->chunk, mesh);
    wgQueueCleanElement(element->id);
}

#if defined(_WIN32) || defined(_WIN64)
    DWORD WINAPI wgThreadWIN32(LPVOID arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueuePop(&element)) {
                wgThreadMethod(element);
            }
            Sleep(1);
        }

        return 0;
    }
#else
    void* wgThreadPOSIX(void* arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueuePop(&element)) {
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

void wgQueueCleanElement(U16 index) {
    if (queue == NULL) return;
    WG_QUEUE_ELEM* element = queue->elements[index];
    if (element == NULL) return;
    queue->elements[index]->used = 0;
    queue->elements[index]->chunk = NULL;
}

void wgQueuePush(CHUNK* chunk) {
    if (queue == NULL) return;

    #if defined(_WIN32) || defined(_WIN64)
        EnterCriticalSection(&wgQueueMutex);
    #else
        pthread_mutex_lock(&wgQueueMutex);
    #endif
    
    if (queue->elements[queue->pushIndex]->used) {
        wgQueueCleanElement(queue->pushIndex);
    }
    queue->elements[queue->pushIndex]->chunk = chunk;
    queue->elements[queue->pushIndex]->id = queue->pushIndex;
    queue->elements[queue->pushIndex]->used = 1;

    queue->pushIndex++;
    if (queue->pushIndex >= queue->size) {
        queue->pushIndex = 0;
    }

    #if defined(_WIN32) || defined(_WIN64)
        LeaveCriticalSection(&wgQueueMutex);
    #else
        pthread_mutex_unlock(&wgQueueMutex);
    #endif
}

U8 wgQueuePop(WG_QUEUE_ELEM** element) {
    if (queue == NULL) return 0;

    #if defined(_WIN32) || defined(_WIN64)
        EnterCriticalSection(&wgQueueMutex);
    #else
        pthread_mutex_lock(&wgQueueMutex);
    #endif
    
    WG_QUEUE_ELEM* elem = queue->elements[queue->popIndex];
    *element = elem;
    if (elem == NULL || !elem->used) {
        #if defined(_WIN32) || defined(_WIN64)
            LeaveCriticalSection(&wgQueueMutex);
        #else
            pthread_mutex_unlock(&wgQueueMutex);
        #endif
        return 0;
    }

    queue->popIndex++;
    if (queue->popIndex >= queue->size) {
        queue->popIndex = 0;
    }

    #if defined(_WIN32) || defined(_WIN64)
        LeaveCriticalSection(&wgQueueMutex);
    #else
        pthread_mutex_unlock(&wgQueueMutex);
    #endif
    return 1;
}

void wgFreeQueue() {
    for (int i = 0; i < WG_QUEUE_SIZE; i++) {
        wgQueueCleanElement(i);
        free(queue->elements[i]);
    }
    free(queue->elements);
    free(queue);
    queue = NULL;

    #if defined(_WIN32) || defined(_WIN64)
        DeleteCriticalSection(&wgQueueMutex);
    #else
        pthread_mutex_destroy(&wgQueueMutex);
    #endif
}

void wgInitQueue() {
    WG_QUEUE* q = malloc(sizeof(WG_QUEUE));
    q->size = WG_QUEUE_SIZE;
    q->pushIndex = 0;
    q->popIndex = 0;
    q->elements = malloc(sizeof(WG_QUEUE_ELEM*) * WG_QUEUE_SIZE);
    for (int i = 0; i < WG_QUEUE_SIZE; i++) {
        q->elements[i] = malloc(sizeof(WG_QUEUE_ELEM));
        q->elements[i]->chunk = NULL;
        q->elements[i]->id = 0;
        q->elements[i]->used = 0;
    }
    queue = q;

    #if defined(_WIN32) || defined(_WIN64)
        InitializeCriticalSection(&wgQueueMutex);
    #else
        pthread_mutex_init(&wgQueueMutex, NULL);
    #endif
}

void wgCleanThreadPool() {
    running = 0;

    if (queue != NULL) wgFreeQueue();
    if (vaoQueue != NULL) wgFreeVAOQueue();
}

void wgInitThreadPool() {
    if (queue == NULL) wgInitQueue();
    if (vaoQueue == NULL) wgInitVAOQueue();

    running = 1;
    #if defined(_WIN32) || defined(_WIN64)
        wgInitThreadPoolWIN32();
    #else
        wgInitThreadPoolPOSIX();
    #endif
}
