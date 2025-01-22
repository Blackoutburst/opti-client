#pragma once

#include "utils/types.h"

typedef struct chunk CHUNK;

#define CHUNK_SIZE 16
#define BLOCK_COUNT CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

struct chunk {
    U32 vaoID;
    U32 vboID;
    U32 eboID;
    U32 meshVertexCount;
    U32 vertexCount;
    I32* position;
    U8* blocks;
};

void chunkPrint(CHUNK* chunk);
I32 chunkPackVertexData(I8 x, I8 y, I8 z, I8 u, I8 v, I8 n, I8 t);
void chunkGenerateVAO(CHUNK * chunk, I32* mesh);
U8 chunkIsMonotype(CHUNK* chunk);
I32* chunkGenerateMesh(CHUNK* chunk);
CHUNK* chunkCreate(I32* position, U8* blocks);
void chunkRender(CHUNK* chunk);
void chunkDrestroy(CHUNK* chunk);
U32 chunkHash(I32 x, I32 y, I32 z);

