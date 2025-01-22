#include <stdlib.h>
#include <stdio.h>
#include "graphics/opengl.h"
#include "utils/types.h"
#include "utils/math.h"
#include "world/blocks.h"
#include "world/world.h"
#include "world/chunk.h"

void chunkPrint(CHUNK* chunk) {
    printf("vao: %i, vbo: %i, ebo: %i, vertex count: %u position: %p blocks: %p\n", chunk->vaoID, chunk->vboID, chunk->eboID, chunk->vertexCount, chunk->position, chunk->blocks);
}

I32 chunkPackVertexData(I8 x, I8 y, I8 z, I8 u, I8 v, I8 n, I8 t) {
    return (x & 31) | (y & 31) << 5 | (z & 31) << 10 | (u & 1) << 15 | (v & 1) << 16 | (n & 7) << 17 | (t & 31) << 20;
}

void chunkGenerateVAO(CHUNK* chunk, I32* vertices) {
    if (!chunk->meshVertexCount) {
        free(vertices);
        return;
    }
    
    glBindVertexArray(chunk->vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(I32) * chunk->meshVertexCount, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_INT, 4, 0);
    free(vertices);

    chunk->vertexCount = chunk->meshVertexCount;
}

U8 chunkIsMonotype(CHUNK* chunk) {
    I32 previous = chunk->blocks[0];
    
    for (I32 i = BLOCK_COUNT - 1; i; i--) {
        if (previous != chunk->blocks[i])
            return 0;
        previous = chunk->blocks[i];
    }

    return 1;
}

I32* chunkGenerateMesh(CHUNK* chunk) {
    I32* vertices = malloc(sizeof(I32) * BLOCK_COUNT * 36);
    I8* blockPos = malloc(sizeof(I8) * 3);
    I32 vertexIndex = 0;
    
    for (U16 i = 0; i < BLOCK_COUNT; i++) {
        I32 blockType = chunk->blocks[i];
        if (!blockType) continue;
        
        indexToXYZ(blockPos, i);

        // TOP
        U32 topIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY] + 1, blockPos[VZ]);
        if (
            (topIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX], chunk->position[VY] + blockPos[VY] + 1, chunk->position[VZ] + blockPos[VZ]))) || 
            (topIndex < BLOCK_COUNT && blocksIsTransparent(chunk->blocks[topIndex]))
        ) {
            vertices[vertexIndex  ] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 0, blocksTextureFace(blockType, 0));
            vertices[vertexIndex+1] = chunkPackVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 0, blocksTextureFace(blockType, 0));
            vertices[vertexIndex+2] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 1, 0, blocksTextureFace(blockType, 0));

            vertices[vertexIndex+3] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 1, 0, blocksTextureFace(blockType, 0));
            vertices[vertexIndex+4] = chunkPackVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 0, blocksTextureFace(blockType, 0));
            vertices[vertexIndex+5] = chunkPackVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 1, 0, blocksTextureFace(blockType, 0));
            vertexIndex += 6;
        }

        // FRONT
        U32 frontIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY], blockPos[VZ] - 1);
        if (
            (frontIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX], chunk->position[VY] + blockPos[VY], chunk->position[VZ] + blockPos[VZ] - 1))) || 
            (frontIndex < BLOCK_COUNT && blocksIsTransparent(chunk->blocks[frontIndex]))
        ) {
            vertices[vertexIndex  ] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 1, blocksTextureFace(blockType, 1));
            vertices[vertexIndex+1] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 1, blocksTextureFace(blockType, 1));
            vertices[vertexIndex+2] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 1, blocksTextureFace(blockType, 1));

            vertices[vertexIndex+3] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 1, blocksTextureFace(blockType, 1));
            vertices[vertexIndex+4] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 1, blocksTextureFace(blockType, 1));
            vertices[vertexIndex+5] = chunkPackVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 1, blocksTextureFace(blockType, 1));
            vertexIndex += 6;
        }

        // BACK
        U32 backIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY], blockPos[VZ] + 1);
        if (
            (backIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX], chunk->position[VY] + blockPos[VY], chunk->position[VZ] + blockPos[VZ] + 1))) || 
            (backIndex < BLOCK_COUNT && blocksIsTransparent(chunk->blocks[backIndex]))
        ) {
            vertices[vertexIndex  ] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 2, blocksTextureFace(blockType, 2));
            vertices[vertexIndex+1] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 1, 2, blocksTextureFace(blockType, 2));
            vertices[vertexIndex+2] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 2, blocksTextureFace(blockType, 2));

            vertices[vertexIndex+3] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 2, blocksTextureFace(blockType, 2));
            vertices[vertexIndex+4] = chunkPackVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 2, blocksTextureFace(blockType, 2));
            vertices[vertexIndex+5] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 2, blocksTextureFace(blockType, 2));
            vertexIndex += 6;
        }

        // LEFT
        U32 leftIndex = xyzToIndexOobCheck(blockPos[VX] - 1, blockPos[VY], blockPos[VZ]);
        if (
            (leftIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX] - 1, chunk->position[VY] + blockPos[VY], chunk->position[VZ] + blockPos[VZ]))) || 
            (leftIndex < BLOCK_COUNT && blocksIsTransparent(chunk->blocks[leftIndex]))
        ) {
            vertices[vertexIndex  ] = chunkPackVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 3, blocksTextureFace(blockType, 3));
            vertices[vertexIndex+1] = chunkPackVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 3, blocksTextureFace(blockType, 3));
            vertices[vertexIndex+2] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 3, blocksTextureFace(blockType, 3));

            vertices[vertexIndex+3] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 3, blocksTextureFace(blockType, 3));
            vertices[vertexIndex+4] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 3, blocksTextureFace(blockType, 3));
            vertices[vertexIndex+5] = chunkPackVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 3, blocksTextureFace(blockType, 3));
            vertexIndex += 6;
        }

        // RIGHT
        U32 rightIndex = xyzToIndexOobCheck(blockPos[VX] + 1, blockPos[VY], blockPos[VZ]);
        if (
            (rightIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX] + 1, chunk->position[VY] + blockPos[VY], chunk->position[VZ] + blockPos[VZ]))) || 
            (rightIndex < BLOCK_COUNT && blocksIsTransparent(chunk->blocks[rightIndex]))
        ) {
            vertices[vertexIndex  ] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 4, blocksTextureFace(blockType, 4));
            vertices[vertexIndex+1] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 4, blocksTextureFace(blockType, 4));
            vertices[vertexIndex+2] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 4, blocksTextureFace(blockType, 4));

            vertices[vertexIndex+3] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 4, blocksTextureFace(blockType, 4));
            vertices[vertexIndex+4] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 1, 4, blocksTextureFace(blockType, 4));
            vertices[vertexIndex+5] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 4, blocksTextureFace(blockType, 4));
            vertexIndex += 6;
        }

        // BOTTOM
        U32 bottomIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY] - 1, blockPos[VZ]);
        if (
            (bottomIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX], chunk->position[VY] + blockPos[VY] - 1, chunk->position[VZ] + blockPos[VZ]))) || 
            (bottomIndex < BLOCK_COUNT && blocksIsTransparent(chunk->blocks[bottomIndex]))
        ) {
            vertices[vertexIndex  ] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 0, 1, 5, blocksTextureFace(blockType, 5));
            vertices[vertexIndex+1] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 1, 1, 5, blocksTextureFace(blockType, 5));
            vertices[vertexIndex+2] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 0, 5, blocksTextureFace(blockType, 5));

            vertices[vertexIndex+3] = chunkPackVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 0, 5, blocksTextureFace(blockType, 5));
            vertices[vertexIndex+4] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 0, 5, blocksTextureFace(blockType, 5));
            vertices[vertexIndex+5] = chunkPackVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 0, 1, 5, blocksTextureFace(blockType, 5));
            vertexIndex += 6;
        }
    }

    free(blockPos);
    chunk->meshVertexCount = vertexIndex;
    return vertices;
}

CHUNK* chunkCreate(I32* position, U8* blocks) {
    CHUNK* chunk = malloc(sizeof(CHUNK));

    glGenVertexArrays(1, &chunk->vaoID);
    glGenBuffers(1, &chunk->vboID);
    glGenBuffers(1, &chunk->eboID);

    chunk->meshVertexCount = 0;
    chunk->vertexCount = 0;
    chunk->position = position;
    chunk->blocks = blocks;

    return chunk;
}

void chunkRender(CHUNK* chunk) {
    if (chunk == NULL) return;
    if (!chunk->vertexCount) return;
    
    glBindVertexArray(chunk->vaoID);
    glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
}

void chunkDrestroy(CHUNK* chunk) {
    if (chunk == NULL) return;
    
    glDeleteVertexArrays(1, &chunk->vaoID);
    glDeleteBuffers(1, &chunk->vboID);
    glDeleteBuffers(1, &chunk->eboID);
    free(chunk->position);
    free(chunk->blocks);
    free(chunk);
    chunk = NULL;
}

U32 chunkHash(I32 x, I32 y, I32 z) {
    U32 h = 2166136261u;
    h = (h ^ (U32)x) * 16777619u;
    h = (h ^ (U32)y) * 16777619u;
    h = (h ^ (U32)z) * 16777619u;
    
    return h;
}
