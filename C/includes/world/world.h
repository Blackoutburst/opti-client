#pragma once

#include "utils/types.h"
#include "world/chunk.h"

#define CHUNK_SIZE 16
#define CHUNK_COUNT 16

void worldAddChunk(CHUNK* chunk);
void worldRemoveChunk(I32 x, I32 y, I32 z);
void worldRender(I32 shaderProgram);
void worldClean();
void worldInit();
