#pragma once

#include "utils/types.h"
#include "world/chunk.h"

typedef struct vaoQueue VAO_QUEUE;
typedef struct vaoQueueElement VAO_QUEUE_ELEM;

#define VAO_QUEUE_SIZE 4096

struct vaoQueueElement {
    U8 used;
    U16 id;
    CHUNK* chunk;
    I32* mesh;
};

struct vaoQueue {
    U16 size;
    U16 pushIndex;
    U16 popIndex;
    VAO_QUEUE_ELEM** elements;
};

void vaoQueueCleanElement(U16 index);
void vaoQueuePush(CHUNK* chunk, I32* mesh);
U8 vaoQueuePop(VAO_QUEUE_ELEM** element);
void vaoQueueFree();
void vaoQueueInit();
