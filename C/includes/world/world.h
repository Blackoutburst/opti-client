#pragma once

#include "utils/types.h"
#include "world/chunk.h"

#define CHUNK_COUNT 64000

typedef struct hashSlot HASH;

struct hashSlot {
    I32 x;
    I32 y;
    I32 z;
    CHUNK* chunk;
    U8 used;
};

CHUNK* _worldGetChunk(I32 x, I32 y, I32 z);
CHUNK* worldGetChunk(I32 x, I32 y, I32 z);
U8 worldGetBlock(I32 x, I32 y, I32 z);
void worldAddChunk(CHUNK* chunk);
void _worldRemoveChunk(I32 x, I32 y, I32 z);
void worldRemoveChunk(I32 x, I32 y, I32 z);
void worldRemoveChunkOutOfRenderDistance(U8 renderDistance, I32 x, I32 y, I32 z);
void worldRender(I32 shaderProgram);
void worldClean();
void worldInit();
