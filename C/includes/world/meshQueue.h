#pragma once

#include "utils/types.h"
#include "utils/vectori.h"
#include "world/chunk.h"

typedef struct meshQueue MESH_QUEUE;
typedef struct meshQueueElement MESH_QUEUE_ELEM;

#define MESH_QUEUE_SIZE 64000

struct meshQueueElement {
    U8 used;
    U16 id;
    VECTORI* position;
    U8* blocks;
};

struct meshQueue {
    U16 size;
    U16 pushIndex;
    U16 popIndex;
    MESH_QUEUE_ELEM** elements;
};

void meshQueueCleanElement(U16 index);
void meshQueuePush(VECTORI* position, U8* blocks);
U8 meshQueuePop(MESH_QUEUE_ELEM** element);
void meshQueueFree();
void meshQueueInit();
