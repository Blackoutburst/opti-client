#pragma once

#include "utils/types.h"
#include "utils/vectori.h"
#include "world/chunk.h"

typedef struct vaoQueue VAO_QUEUE;
typedef struct vaoQueueElement VAO_QUEUE_ELEM;

#define VAO_QUEUE_SIZE 64000

struct vaoQueueElement {
    U8 used;
    U16 id;
    VECTORI* position;
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
void vaoQueuePush(VECTORI* position, U8* blocks, CHUNK_MESH* mesh);
U8 vaoQueuePop(VAO_QUEUE_ELEM** element);
void vaoQueueFree();
void vaoQueueInit();
