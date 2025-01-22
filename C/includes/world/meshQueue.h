#pragma once

#include "utils/types.h"
#include "world/chunk.h"

typedef struct meshQueue MESH_QUEUE;
typedef struct meshQueueElement MESH_QUEUE_ELEM;

#define MESH_QUEUE_SIZE 4096 * 2

struct meshQueueElement {
    U8 used;
    U16 id;
    CHUNK* chunk;
};

struct meshQueue {
    U16 size;
    U16 pushIndex;
    U16 popIndex;
    MESH_QUEUE_ELEM** elements;
};

void meshQueueCleanElement(U16 index);
void meshQueuePush(CHUNK* chunk);
U8 meshQueuePop(MESH_QUEUE_ELEM** element);
void meshQueueFree();
void meshQueueInit();
