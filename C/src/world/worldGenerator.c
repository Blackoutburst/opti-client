#include <stdlib.h>
#include "utils/ioUtils.h"
#include "utils/math.h"
#include "utils/types.h"
#include "world/worldGenerator.h"
#include "world/world.h"

static U8 running = 0;

static WG_QUEUE* queue = NULL;
static WG_VAO_QUEUE* vaoQueue = NULL;

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
}

U8 wgVAOQueuePop(WG_VAO_QUEUE_ELEM** element) {
    if (vaoQueue == NULL) return 0;
    WG_VAO_QUEUE_ELEM* elem = vaoQueue->elements[vaoQueue->popIndex];
    *element = elem;
    if (elem == NULL || !elem->used) return 0;

    vaoQueue->popIndex++;
    if (vaoQueue->popIndex >= vaoQueue->size) {
        vaoQueue->popIndex = 0;
    }

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
}

void wgThreadMethod(WG_QUEUE_ELEM* element) {
    // CENTER
    I32* mesh = generateChunkMesh(element->chunk);
    wgVAOQueuePush(element->chunk, mesh);
    // TOP
    CHUNK* top = worldGetChunk(element->chunk->position[VX], element->chunk->position[VY] + CHUNK_SIZE, element->chunk->position[VZ]);
    if (top != NULL) {
        I32* m = generateChunkMesh(top);
        wgVAOQueuePush(top, m);
    }

    // FRONT
    CHUNK* front = worldGetChunk(element->chunk->position[VX], element->chunk->position[VY], element->chunk->position[VZ] - CHUNK_SIZE);
    if (front != NULL) {
        I32* m = generateChunkMesh(front);
        wgVAOQueuePush(front, m);
    }

    // BACK
    CHUNK* back = worldGetChunk(element->chunk->position[VX], element->chunk->position[VY], element->chunk->position[VZ] + CHUNK_SIZE);
    if (back != NULL) {
        I32* m = generateChunkMesh(back);
        wgVAOQueuePush(back, m);
    }

    // LEFT
    CHUNK* left = worldGetChunk(element->chunk->position[VX] - CHUNK_SIZE, element->chunk->position[VY], element->chunk->position[VZ]);
    if (left != NULL) {
        I32* m = generateChunkMesh(left);
        wgVAOQueuePush(left, m);
    }

    // RIGHT
    CHUNK* right = worldGetChunk(element->chunk->position[VX] + CHUNK_SIZE, element->chunk->position[VY], element->chunk->position[VZ]);
    if (right != NULL) {
        I32* m = generateChunkMesh(right);
        wgVAOQueuePush(right, m);
    }

    // BOTTOM
    CHUNK* bottom = worldGetChunk(element->chunk->position[VX], element->chunk->position[VY] - CHUNK_SIZE, element->chunk->position[VZ]);
    if (bottom != NULL) {
        I32* m = generateChunkMesh(bottom);
        wgVAOQueuePush(bottom, m);
    }
    
    wgQueueCleanElement(element->id);
}

#if defined(_WIN32) || defined(_WIN64)
    DWORD WINAPI wgThreadWIN32(LPVOID arg) {
        WG_QUEUE_ELEM* element = NULL;
        while (running) {
            while(wgQueuePop(&element)) {
                wgThreadMethod(element);
            }
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
        }
        
        return NULL;
    }
#endif

#if defined(_WIN32) || defined(_WIN64)
    void wgInitThreadPoolWIN32() {
        HANDLE thread = CreateThread(NULL, 0, wgThreadWIN32, NULL, 0, NULL);
        if (thread == NULL) {
            println("World gen thread creation failed");
            return;
        }
        CloseHandle(thread);
    }
#else
    void wgInitThreadPoolPOSIX() {
        pthread_t thread;
        if (pthread_create(&thread, NULL, wgThreadPOSIX, NULL)) {
            println("World gen thread creation failed");
            return;
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
}

U8 wgQueuePop(WG_QUEUE_ELEM** element) {
    if (queue == NULL) return 0;
    WG_QUEUE_ELEM* elem = queue->elements[queue->popIndex];
    *element = elem;
    if (elem == NULL || !elem->used) return 0;

    queue->popIndex++;
    if (queue->popIndex >= queue->size) {
        queue->popIndex = 0;
    }
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
