#include <stdlib.h>
#include "utils/types.h"
#include "utils/mutex.h"
#include "world/chunk.h"
#include "world/meshQueue.h"

static MESH_QUEUE* queue = NULL;

#if defined(_WIN32) || defined(_WIN64)
    static CRITICAL_SECTION mutex;
#else
    static pthread_mutex_t mutex;
#endif

void meshQueueCleanElement(U16 index) {
    if (queue == NULL) return;
    
    MESH_QUEUE_ELEM* element = queue->elements[index];
    if (element == NULL) return;
    
    queue->elements[index]->used = 0;
    queue->elements[index]->neighbor = 0;
    queue->elements[index]->position = NULL;
    queue->elements[index]->blocks = NULL;
}

void meshQueuePush(VECTORI* position, U8* blocks, U8 neighbor) {
    if (queue == NULL) return;

    mutexLock(&mutex);
    
    if (queue->elements[queue->pushIndex]->used) {
        meshQueueCleanElement(queue->pushIndex);
    }
    
    queue->elements[queue->pushIndex]->position = position;
    queue->elements[queue->pushIndex]->blocks = blocks;
    queue->elements[queue->pushIndex]->id = queue->pushIndex;
    queue->elements[queue->pushIndex]->used = 1;
    queue->elements[queue->pushIndex]->neighbor = neighbor;

    queue->pushIndex++;
    
    if (queue->pushIndex >= queue->size) {
        queue->pushIndex = 0;
    }

    mutexUnlock(&mutex);
}

U8 meshQueuePop(MESH_QUEUE_ELEM** element) {
    if (queue == NULL) return 0;

    mutexLock(&mutex);
    
    MESH_QUEUE_ELEM* elem = queue->elements[queue->popIndex];
    *element = elem;
    
    if (elem == NULL || !elem->used) {
        mutexUnlock(&mutex);
        return 0;
    }

    queue->popIndex++;
    
    if (queue->popIndex >= queue->size) {
        queue->popIndex = 0;
    }

    mutexUnlock(&mutex);
    return 1;
}

void meshQueueFree() {
    if (queue == NULL) return;

    for (int i = 0; i < MESH_QUEUE_SIZE; i++) {
        meshQueueCleanElement(i);
        free(queue->elements[i]);
    }
    free(queue->elements);
    free(queue);
    queue = NULL;

    mutexDestroy(&mutex);
}

void meshQueueInit() {
    if (queue != NULL) return;

    MESH_QUEUE* q = malloc(sizeof(MESH_QUEUE));
    q->size = MESH_QUEUE_SIZE;
    q->pushIndex = 0;
    q->popIndex = 0;
    q->elements = malloc(sizeof(MESH_QUEUE_ELEM*) * MESH_QUEUE_SIZE);
    for (int i = 0; i < MESH_QUEUE_SIZE; i++) {
        q->elements[i] = malloc(sizeof(MESH_QUEUE_ELEM));
        q->elements[i]->position = NULL;
        q->elements[i]->blocks = NULL;
        q->elements[i]->id = 0;
        q->elements[i]->used = 0;
        q->elements[i]->neighbor = 0;
    }
    queue = q;

    mutexCreate(&mutex);
}
