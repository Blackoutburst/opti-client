#pragma once

#include "utils/types.h"
#include "world/chunk.h"

#define CHUNK_SIZE 16
#define CHUNK_COUNT 4096

typedef struct hashSlot HASH;

struct hashSlot {
    I32 x;
    I32 y;
    I32 z;
    CHUNK* chunk;
    U8 used;
};

CHUNK* worldGetChunk(I32 x, I32 y, I32 z);
U8 worldGetBlock(I32 x, I32 y, I32 z);
void worldAddChunk(CHUNK* chunk);
void worldRemoveChunk(I32 x, I32 y, I32 z);
void worldRender(I32 shaderProgram);
void worldClean();
void worldInit();
