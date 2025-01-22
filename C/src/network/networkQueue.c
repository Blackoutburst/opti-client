#include <stdlib.h>
#include "network/networkQueue.h"

static NET_QUEUE* networkQueue = NULL;

void networkQueueCleanElement(U16 index) {
    if (networkQueue == NULL) return;
    
    NET_QUEUE_ELEM* element = networkQueue->elements[index];
    if (element == NULL) return;
    
    free(element->buffer);
    
    element->buffer = NULL;
    element->function = NULL;
    networkQueue->elements[index]->used = 0;
}

void networkQueuePush(void (*function)(U8*), U8* buffer) {
    if (networkQueue == NULL) return;
    
    if (networkQueue->elements[networkQueue->pushIndex]->used) {
        networkQueueCleanElement(networkQueue->pushIndex);
    }
    
    networkQueue->elements[networkQueue->pushIndex]->function = function;
    networkQueue->elements[networkQueue->pushIndex]->buffer = buffer;
    networkQueue->elements[networkQueue->pushIndex]->id = networkQueue->pushIndex;
    networkQueue->elements[networkQueue->pushIndex]->used = 1;

    networkQueue->pushIndex++;
    
    if (networkQueue->pushIndex >= networkQueue->size) {
        networkQueue->pushIndex = 0;
    }
}

U8 networkQueuePop(NET_QUEUE_ELEM** element) {
    if (networkQueue == NULL) return 0;
    
    NET_QUEUE_ELEM* elem = networkQueue->elements[networkQueue->popIndex];
    *element = elem;
    
    if (elem == NULL || !elem->used) return 0;

    networkQueue->popIndex++;
    
    if (networkQueue->popIndex >= networkQueue->size) {
        networkQueue->popIndex = 0;
    }

    return 1;
}

void networkQueueInit() {
    if (networkQueue != NULL) return;
    
    NET_QUEUE* queue = malloc(sizeof(NET_QUEUE));
    queue->size = NET_QUEUE_SIZE;
    queue->pushIndex = 0;
    queue->popIndex = 0;
    queue->elements = malloc(sizeof(NET_QUEUE_ELEM*) * NET_QUEUE_SIZE);
    for (int i = 0; i < NET_QUEUE_SIZE; i++) {
        queue->elements[i] = malloc(sizeof(NET_QUEUE_ELEM));
        queue->elements[i]->function = NULL;
        queue->elements[i]->buffer = NULL;
        queue->elements[i]->id = 0;
        queue->elements[i]->used = 0;
    }
    
    networkQueue = queue;
}

void networkQueueClean() {
    if (networkQueue == NULL) return;
    
    for (int i = 0; i < NET_QUEUE_SIZE; i++) {
        networkQueueCleanElement(i);
        free(networkQueue->elements[i]);
    }
    
    free(networkQueue->elements);
    free(networkQueue);
    networkQueue = NULL;
}
