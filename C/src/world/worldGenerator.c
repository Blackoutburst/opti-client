#include "utils/ioUtils.h"
#include "utils/math.h"
#include "utils/types.h"
#include "world/worldGenerator.h"
#include "world/world.h"

static U8 running = 0;

static WG_QUEUE* queue1 = NULL;
static WG_QUEUE* queue2 = NULL;
static WG_QUEUE* queue3 = NULL;
static WG_QUEUE* queue4 = NULL;

static WG_VAO_QUEUE* vaoQueue1 = NULL;
static WG_VAO_QUEUE* vaoQueue2 = NULL;
static WG_VAO_QUEUE* vaoQueue3 = NULL;
static WG_VAO_QUEUE* vaoQueue4 = NULL;

void _wgVAOQueueCleanElement(WG_VAO_QUEUE* queue, U16 index) {
    if (queue == NULL) return;
    WG_VAO_QUEUE_ELEM* element = queue->elements[index];
    if (element == NULL) return;
    free(element);
    queue->elements[index] = NULL;
}

void wgVAOQueue1CleanElement(U16 index) {
    _wgVAOQueueCleanElement(vaoQueue1, index);
}

void wgVAOQueue2CleanElement(U16 index) {
    _wgVAOQueueCleanElement(vaoQueue2, index);
}

void wgVAOQueue3CleanElement(U16 index) {
    _wgVAOQueueCleanElement(vaoQueue3, index);
}

void wgVAOQueue4CleanElement(U16 index) {
    _wgVAOQueueCleanElement(vaoQueue4, index);
}

void wgVAOQueuePush(WG_VAO_QUEUE* queue, CHUNK* chunk, I32* mesh) {
    if (queue == NULL) return;
    if (queue->elements[queue->pushIndex] != NULL) {
        _wgVAOQueueCleanElement(queue, queue->pushIndex);
    }
    queue->elements[queue->pushIndex] = malloc(sizeof(WG_VAO_QUEUE_ELEM));
    queue->elements[queue->pushIndex]->chunk = chunk;
    queue->elements[queue->pushIndex]->mesh = mesh;
    queue->elements[queue->pushIndex]->id = queue->pushIndex;

    queue->pushIndex++;
    if (queue->pushIndex >= queue->size) {
        queue->pushIndex = 0;
    }
}

U8 _wgVAOQueuePop(WG_VAO_QUEUE* queue, WG_VAO_QUEUE_ELEM** element) {
    if (queue == NULL) return 0;
    WG_VAO_QUEUE_ELEM* elem = queue->elements[queue->popIndex];
    if (elem == NULL) return 0;

    queue->popIndex++;
    if (queue->popIndex >= queue->size) {
        queue->popIndex = 0;
    }

    *element = elem;
    return 1;
}

U8 wgVAOQueue1Pop(WG_VAO_QUEUE_ELEM** element) {
    return _wgVAOQueuePop(vaoQueue1, element);
}

U8 wgVAOQueue2Pop(WG_VAO_QUEUE_ELEM** element) {
    return _wgVAOQueuePop(vaoQueue2, element);
}

U8 wgVAOQueue3Pop(WG_VAO_QUEUE_ELEM** element) {
    return _wgVAOQueuePop(vaoQueue3, element);
}

U8 wgVAOQueue4Pop(WG_VAO_QUEUE_ELEM** element) {
    return _wgVAOQueuePop(vaoQueue4, element);
}

void wgFreeVAOQueue(WG_VAO_QUEUE* queue) {
    for (int i = 0; i < WG_VAO_QUEUE_SIZE; i++) _wgVAOQueueCleanElement(queue, i);
    free(queue->elements);
    free(queue);
    queue = NULL;
}

void wgInitVAOQueue(WG_VAO_QUEUE** queue) {
    (*queue) = malloc(sizeof(WG_VAO_QUEUE));
    (*queue)->size = WG_VAO_QUEUE_SIZE;
    (*queue)->pushIndex = 0;
    (*queue)->popIndex = 0;
    (*queue)->elements = malloc(sizeof(WG_VAO_QUEUE_ELEM*) * WG_VAO_QUEUE_SIZE);
    for (int i = 0; i < WG_VAO_QUEUE_SIZE; i++) (*queue)->elements[i] = NULL;
}

void wgThreadMethod(WG_QUEUE* queue, WG_VAO_QUEUE* vaoQueue, WG_QUEUE_ELEM* element) {
    // CENTER
    I32* mesh = generateChunkMesh(element->chunk);
    wgVAOQueuePush(vaoQueue, element->chunk, mesh);

    // TOP
    CHUNK* top = worldGetChunk(element->chunk->position[VX], element->chunk->position[VY] + CHUNK_SIZE, element->chunk->position[VZ]);
    if (top != NULL) {
        I32* m = generateChunkMesh(top);
        wgVAOQueuePush(vaoQueue, top, m);
    }

    // FRONT
    CHUNK* front = worldGetChunk(element->chunk->position[VX], element->chunk->position[VY], element->chunk->position[VZ] - CHUNK_SIZE);
    if (front != NULL) {
        I32* m = generateChunkMesh(front);
        wgVAOQueuePush(vaoQueue, front, m);
    }

    // BACK
    CHUNK* back = worldGetChunk(element->chunk->position[VX], element->chunk->position[VY], element->chunk->position[VZ] + CHUNK_SIZE);
    if (back != NULL) {
        I32* m = generateChunkMesh(back);
        wgVAOQueuePush(vaoQueue, back, m);
    }

    // LEFT
    CHUNK* left = worldGetChunk(element->chunk->position[VX] - CHUNK_SIZE, element->chunk->position[VY], element->chunk->position[VZ]);
    if (left != NULL) {
        I32* m = generateChunkMesh(left);
        wgVAOQueuePush(vaoQueue, left, m);
    }

    // RIGHT
    CHUNK* right = worldGetChunk(element->chunk->position[VX] + CHUNK_SIZE, element->chunk->position[VY], element->chunk->position[VZ]);
    if (right != NULL) {
        I32* m = generateChunkMesh(right);
        wgVAOQueuePush(vaoQueue, right, m);
    }

    // BOTTOM
    CHUNK* bottom = worldGetChunk(element->chunk->position[VX], element->chunk->position[VY] - CHUNK_SIZE, element->chunk->position[VZ]);
    if (bottom != NULL) {
        I32* m = generateChunkMesh(bottom);
        wgVAOQueuePush(vaoQueue, bottom, m);
    }
    
    wgQueueCleanElement(queue, element->id);
}

#if defined(_WIN32) || defined(_WIN64)
    DWORD WINAPI wgThread4WIN32(LPVOID arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueue4Pop(&element)) {
                wgThreadMethod(queue4, vaoQueue4, element);
            }
        }

        return 0;
    }

    DWORD WINAPI wgThread3WIN32(LPVOID arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueue3Pop(&element)) {
                wgThreadMethod(queue3, vaoQueue3, element);
            }
        }

        return 0;
    }

    DWORD WINAPI wgThread2WIN32(LPVOID arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueue2Pop(&element)) {
                wgThreadMethod(queue2, vaoQueue2, element);
            }
        }

        return 0;
    }

    DWORD WINAPI wgThread1WIN32(LPVOID arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueue1Pop(&element)) {
                wgThreadMethod(queue1, vaoQueue1, element);
            }
        }

        return 0;
    }
#else
    void* wgThread4POSIX(void* arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueue4Pop(&element)) {
                wgThreadMethod(queue4, vaoQueue4, element);
            }
        }
        
        return NULL;
    }

    void* wgThread3POSIX(void* arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueue3Pop(&element)) {
                wgThreadMethod(queue3, vaoQueue3, element);
            }
        }
        
        return NULL;
    }

    void* wgThread2POSIX(void* arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueue2Pop(&element)) {
                wgThreadMethod(queue2, vaoQueue2, element);
            }
        }
        
        return NULL;
    }

    void* wgThread1POSIX(void* arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueue1Pop(&element)) {
                wgThreadMethod(queue1, vaoQueue1, element);
            }
        }
        
        return NULL;
    }
#endif

#if defined(_WIN32) || defined(_WIN64)
    void wgInitThreadPoolWIN32() {
        HANDLE thread1 = CreateThread(NULL, 0, wgThread1WIN32, NULL, 0, NULL);
        if (thread1 == NULL) {
            println("World gen thread 1 creation failed");
            return;
        }
        CloseHandle(thread1);
        
        HANDLE thread2 = CreateThread(NULL, 0, wgThread2WIN32, NULL, 0, NULL);
        if (thread2 == NULL) {
            println("World gen thread 2 creation failed");
            return;
        }
        CloseHandle(thread2);
        
        HANDLE thread3 = CreateThread(NULL, 0, wgThread3WIN32, NULL, 0, NULL);
        if (thread3 == NULL) {
            println("World gen thread 3 creation failed");
            return;
        }
        CloseHandle(thread3);
        
        HANDLE thread4 = CreateThread(NULL, 0, wgThread4WIN32, NULL, 0, NULL);
        if (thread4 == NULL) {
            println("World gen thread 4 creation failed");
            return;
        }
        CloseHandle(thread4);
    }
#else
    void wgInitThreadPoolPOSIX() {
        pthread_t thread1;
        if (pthread_create(&thread1, NULL, wgThread1POSIX, NULL)) {
            println("World gen thread 1 creation failed");
            return;
        }
        
        pthread_t thread2;
        if (pthread_create(&thread2, NULL, wgThread2POSIX, NULL)) {
            println("World gen thread 2 creation failed");
            return;
        }
        
        pthread_t thread3;
        if (pthread_create(&thread3, NULL, wgThread3POSIX, NULL)) {
            println("World gen thread 3 creation failed");
            return;
        }
        
        pthread_t thread4;
        if (pthread_create(&thread4, NULL, wgThread4POSIX, NULL)) {
            println("World gen thread 4 creation failed");
            return;
        }
    }
#endif

void wgQueueCleanElement(WG_QUEUE* queue, U16 index) {
    if (queue == NULL) return;
    WG_QUEUE_ELEM* element = queue->elements[index];
    if (element == NULL) return;
    free(element);
    queue->elements[index] = NULL;
}

void _wgQueuePush(WG_QUEUE* queue, CHUNK* chunk) {
    if (queue == NULL) return;
    if (queue->elements[queue->pushIndex] != NULL) {
        wgQueueCleanElement(queue, queue->pushIndex);
    }
    queue->elements[queue->pushIndex] = malloc(sizeof(WG_QUEUE_ELEM));
    queue->elements[queue->pushIndex]->chunk = chunk;
    queue->elements[queue->pushIndex]->id = queue->pushIndex;

    queue->pushIndex++;
    if (queue->pushIndex >= queue->size) {
        queue->pushIndex = 0;
    }
}

void wgQueue1Push(CHUNK* chunk) {
    return _wgQueuePush(queue1, chunk);
}

void wgQueue2Push(CHUNK* chunk) {
    return _wgQueuePush(queue2, chunk);
}

void wgQueue3Push(CHUNK* chunk) {
    return _wgQueuePush(queue3, chunk);
}

void wgQueue4Push(CHUNK* chunk) {
    return _wgQueuePush(queue4, chunk);
}

void wgQueuePush(CHUNK* chunk) {
    static U32 index = 0;
    switch(index++ % 4) {
        case 0: return wgQueue1Push(chunk);
        case 1: return wgQueue2Push(chunk);
        case 2: return wgQueue3Push(chunk);
        case 3: return wgQueue4Push(chunk);
    }
}

U8 _wgQueuePop(WG_QUEUE* queue, WG_QUEUE_ELEM** element) {
    if (queue == NULL) return 0;
    WG_QUEUE_ELEM* elem = queue->elements[queue->popIndex];
    if (elem == NULL) return 0;

    queue->popIndex++;
    if (queue->popIndex >= queue->size) {
        queue->popIndex = 0;
    }

    *element = elem;
    return 1;
}

U8 wgQueue1Pop(WG_QUEUE_ELEM** element) {
    return _wgQueuePop(queue1, element);
}

U8 wgQueue2Pop(WG_QUEUE_ELEM** element) {
    return _wgQueuePop(queue2, element);
}

U8 wgQueue3Pop(WG_QUEUE_ELEM** element) {
    return _wgQueuePop(queue3, element);
}

U8 wgQueue4Pop(WG_QUEUE_ELEM** element) {
    return _wgQueuePop(queue4, element);
}

void wgFreeQueue(WG_QUEUE* queue) {
    for (int i = 0; i < WG_QUEUE_SIZE; i++) wgQueueCleanElement(queue, i);
    free(queue->elements);
    free(queue);
    queue = NULL;
}

void wgInitQueue(WG_QUEUE** queue) {
    (*queue) = malloc(sizeof(WG_QUEUE));
    (*queue)->size = WG_QUEUE_SIZE;
    (*queue)->pushIndex = 0;
    (*queue)->popIndex = 0;
    (*queue)->elements = malloc(sizeof(WG_QUEUE_ELEM*) * WG_QUEUE_SIZE);
    for (int i = 0; i < WG_QUEUE_SIZE; i++) (*queue)->elements[i] = NULL;
}

void wgCleanThreadPool() {
    running = 0;

    if (queue1 != NULL) wgFreeQueue(queue1);
    if (queue2 != NULL) wgFreeQueue(queue2);
    if (queue3 != NULL) wgFreeQueue(queue3);
    if (queue4 != NULL) wgFreeQueue(queue4);
}

void wgInitThreadPool() {
    running = 1;
    #if defined(_WIN32) || defined(_WIN64)
        wgInitThreadPoolWIN32();
    #else
        wgInitThreadPoolPOSIX();
    #endif

    if (queue1 == NULL) wgInitQueue(&queue1);
    if (queue2 == NULL) wgInitQueue(&queue2);
    if (queue3 == NULL) wgInitQueue(&queue3);
    if (queue4 == NULL) wgInitQueue(&queue4);

    if (vaoQueue1 == NULL) wgInitVAOQueue(&vaoQueue1);
    if (vaoQueue2 == NULL) wgInitVAOQueue(&vaoQueue2);
    if (vaoQueue3 == NULL) wgInitVAOQueue(&vaoQueue3);
    if (vaoQueue4 == NULL) wgInitVAOQueue(&vaoQueue4);
}
