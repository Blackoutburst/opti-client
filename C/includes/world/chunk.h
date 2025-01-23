#pragma once

#include "utils/types.h"

typedef struct chunk CHUNK;
typedef struct chunkMesh CHUNK_MESH;

#define CHUNK_SIZE 16
#define BLOCK_COUNT CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

struct chunk {
    U32 vaoID;
    U32 vboID;
    U32 eboID;
    U32 vertexCount;
    I32* position;
    U8* blocks;
};

struct chunkMesh {
    U32 vertexCount;
    I32* vertices;
};

void chunkPrint(CHUNK* chunk);
I32 chunkPackVertexData(I8 x, I8 y, I8 z, I8 u, I8 v, I8 n, I8 t);
void chunkGenerateVAO(CHUNK * chunk, CHUNK_MESH* mesh);
U8 _chunkIsMonotype(U8* blocks);
U8 chunkIsMonotype(CHUNK* chunk);
CHUNK_MESH* chunkGenerateMesh(I32* position, U8* blocks);
CHUNK* chunkCreate(I32* position, U8* blocks);
void chunkRender(CHUNK* chunk);
void chunkDrestroy(CHUNK* chunk);
U32 chunkHash(I32 x, I32 y, I32 z);

