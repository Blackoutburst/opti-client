#pragma once

#include "utils/types.h"
#include "world/chunk.h"

typedef struct vaoQueue VAO_QUEUE;
typedef struct vaoQueueElement VAO_QUEUE_ELEM;

#define VAO_QUEUE_SIZE 32768

struct vaoQueueElement {
    U8 used;
    U16 id;
    U8 neighbor;
    I32* position;
    U8* blocks;
    CHUNK_MESH* mesh;
};

struct vaoQueue {
    U16 size;
    U16 pushIndex;
    U16 popIndex;
    VAO_QUEUE_ELEM** elements;
};

void vaoQueueCleanElement(U16 index);
void vaoQueuePush(I32* position, U8* blocks, CHUNK_MESH* mesh, U8 neighbor);
U8 vaoQueuePop(VAO_QUEUE_ELEM** element);
void vaoQueueFree();
void vaoQueueInit();
