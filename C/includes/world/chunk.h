#pragma once
#include "utils/types.h"
typedef struct chunk CHUNK;

struct chunk {
    U32 vaoID;
    U32 vboID;
    U32 eboID;
    U32 vertexCount;
    I32* position;
    U8* blocks;
};

void printChunk(CHUNK* chunk);
I32 packVertexData(I8 x, I8 y, I8 z, I8 u, I8 v, I8 n, I8 t);
void generateChunkVAO(CHUNK * chunk, I32* mesh);
U8 isChunkMonotype(CHUNK* chunk);
U8 isBlockTransparent(U8 blockType);
U8 blockTextureFace(U8 blockType, U8 face);
I32* generateChunkMesh(CHUNK* chunk);
CHUNK* createChunk(I32* position, U8* blocks);
void renderChunk(CHUNK* chunk);
void destroyChunk(CHUNK* chunk);
U32 chunkHash(I32 x, I32 y, I32 z);

