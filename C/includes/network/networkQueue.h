#pragma once

#include "utils/types.h"

#define NET_QUEUE_SIZE 4096

typedef struct networkQueue NET_QUEUE;
typedef struct networkQueueElement NET_QUEUE_ELEM;

struct networkQueueElement {
    U8 used;
    U16 id;
    void (*function)(U8*);
    U8* buffer;
};

struct networkQueue {
    U16 size;
    U16 pushIndex;
    U16 popIndex;
    NET_QUEUE_ELEM** elements;
};

void networkQueueInit();
void networkQueueClean();
void networkQueueCleanElement(U16 index);
void networkQueuePush(void (*function)(U8*), U8* buffer);
U8 networkQueuePop(NET_QUEUE_ELEM** element);
