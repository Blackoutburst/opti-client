#include <stdio.h>
#include <stdlib.h>
#include "utils/types.h"
#include "world/chunk.h"
#include "world/world.h"
#include "world/blocks.h"
#include "utils/math.h"
#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include "GL/glew.h"
    #include <GL/gl.h>
#else
    #include <GL/gl.h>
#endif

#define BLOCK_COUNT CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

void printChunk(CHUNK* chunk) {
    printf("vao: %i, vbo: %i, ebo: %i, vertex count: %u position: %p blocks: %p\n", chunk->vaoID, chunk->vboID, chunk->eboID, chunk->vertexCount, chunk->position, chunk->blocks);
}

I32 packVertexData(I8 x, I8 y, I8 z, I8 u, I8 v, I8 n, I8 t) {
    return (x & 31) | (y & 31) << 5 | (z & 31) << 10 | (u & 31) << 15 | (v & 31) << 20 | (n & 7) << 25 | (t & 15) << 28;
}

void generateChunkVAO(CHUNK* chunk, I32* vertices) {
    if (!chunk->meshVertexCount) return;
    glBindVertexArray(chunk->vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(I32) * chunk->meshVertexCount, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_INT, 4, 0);
    free(vertices);

    chunk->vertexCount = chunk->meshVertexCount;
}

U8 isChunkMonotype(CHUNK* chunk) {
    I32 previous = chunk->blocks[0];
    for (I32 i = BLOCK_COUNT - 1; i; i--) {
        if (previous != chunk->blocks[i])
            return 0;
        previous = chunk->blocks[i];
    }

    return 1;
}

U8 isBlockTransparent(U8 blockType) {
    switch (blockType) {
        case AIR: case OAK_LEAVES: case GLASS: return 1;
        default: return 0;
    }
}

U8 blockTextureFace(U8 blockType, U8 face) {
    switch (blockType) {
        case GRASS: {
            switch (face) {
                case 0: return TEXTURE_GRASS_TOP;
                case 1: case 2: case 3: case 4: return TEXTURE_GRASS_SIDE;
                case 5: return TEXTURE_DIRT;
            }
        }
        case DIRT: return TEXTURE_DIRT;
        case STONE: return TEXTURE_STONE;
        case OAK_LOG: {
            switch (face) {
                case 0: case 5: return TEXTURE_LOG_OAK_TOP;
                case 1: case 2: case 3: case 4: return TEXTURE_LOG_OAK;
            }
        }
        case OAK_LEAVES: return TEXTURE_LEAVES_OAK;
        case GLASS: return TEXTURE_GLASS;
        case WATER: return TEXTURE_WATER;
        case SAND: return TEXTURE_SAND;
        case SNOW: return TEXTURE_SNOW;
        default: return TEXTURE_ERROR;
    }
}

I32* generateChunkMesh(CHUNK* chunk) {
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
            (topIndex >= BLOCK_COUNT && isBlockTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX], chunk->position[VY] + blockPos[VY] + 1, chunk->position[VZ] + blockPos[VZ]))) || 
            (topIndex < BLOCK_COUNT && isBlockTransparent(chunk->blocks[topIndex]))
        ) {
            vertices[vertexIndex] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 0, blockTextureFace(blockType, 0));
            vertices[vertexIndex+1] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 0, blockTextureFace(blockType, 0));
            vertices[vertexIndex+2] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 1, 0, blockTextureFace(blockType, 0));

            vertices[vertexIndex+3] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 1, 0, blockTextureFace(blockType, 0));
            vertices[vertexIndex+4] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 0, blockTextureFace(blockType, 0));
            vertices[vertexIndex+5] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 1, 0, blockTextureFace(blockType, 0));
            vertexIndex += 6;
        }

        // FRONT
        U32 frontIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY], blockPos[VZ] - 1);
        if (
            (frontIndex >= BLOCK_COUNT && isBlockTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX], chunk->position[VY] + blockPos[VY], chunk->position[VZ] + blockPos[VZ] - 1))) || 
            (frontIndex < BLOCK_COUNT && isBlockTransparent(chunk->blocks[frontIndex]))
        ) {
            vertices[vertexIndex] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 1, blockTextureFace(blockType, 1));
            vertices[vertexIndex+1] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 1, blockTextureFace(blockType, 1));
            vertices[vertexIndex+2] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 1, blockTextureFace(blockType, 1));

            vertices[vertexIndex+3] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 1, blockTextureFace(blockType, 1));
            vertices[vertexIndex+4] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 1, blockTextureFace(blockType, 1));
            vertices[vertexIndex+5] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 1, blockTextureFace(blockType, 1));
            vertexIndex += 6;
        }

        // BACK
        U32 backIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY], blockPos[VZ] + 1);
        if (
            (backIndex >= BLOCK_COUNT && isBlockTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX], chunk->position[VY] + blockPos[VY], chunk->position[VZ] + blockPos[VZ] + 1))) || 
            (backIndex < BLOCK_COUNT && isBlockTransparent(chunk->blocks[backIndex]))
        ) {
            vertices[vertexIndex] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 2, blockTextureFace(blockType, 2));
            vertices[vertexIndex+1] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 1, 2, blockTextureFace(blockType, 2));
            vertices[vertexIndex+2] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 2, blockTextureFace(blockType, 2));

            vertices[vertexIndex+3] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 2, blockTextureFace(blockType, 2));
            vertices[vertexIndex+4] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 2, blockTextureFace(blockType, 2));
            vertices[vertexIndex+5] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 2, blockTextureFace(blockType, 2));
            vertexIndex += 6;
        }

        // LEFT
        U32 leftIndex = xyzToIndexOobCheck(blockPos[VX] - 1, blockPos[VY], blockPos[VZ]);
        if (
            (leftIndex >= BLOCK_COUNT && isBlockTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX] - 1, chunk->position[VY] + blockPos[VY], chunk->position[VZ] + blockPos[VZ]))) || 
            (leftIndex < BLOCK_COUNT && isBlockTransparent(chunk->blocks[leftIndex]))
        ) {
            vertices[vertexIndex] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 3, blockTextureFace(blockType, 3));
            vertices[vertexIndex+1] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 3, blockTextureFace(blockType, 3));
            vertices[vertexIndex+2] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 3, blockTextureFace(blockType, 3));

            vertices[vertexIndex+3] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 3, blockTextureFace(blockType, 3));
            vertices[vertexIndex+4] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 3, blockTextureFace(blockType, 3));
            vertices[vertexIndex+5] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 3, blockTextureFace(blockType, 3));
            vertexIndex += 6;
        }

        // RIGHT
        U32 rightIndex = xyzToIndexOobCheck(blockPos[VX] + 1, blockPos[VY], blockPos[VZ]);
        if (
            (rightIndex >= BLOCK_COUNT && isBlockTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX] + 1, chunk->position[VY] + blockPos[VY], chunk->position[VZ] + blockPos[VZ]))) || 
            (rightIndex < BLOCK_COUNT && isBlockTransparent(chunk->blocks[rightIndex]))
        ) {
            vertices[vertexIndex] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 4, blockTextureFace(blockType, 4));
            vertices[vertexIndex+1] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 4, blockTextureFace(blockType, 4));
            vertices[vertexIndex+2] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 4, blockTextureFace(blockType, 4));

            vertices[vertexIndex+3] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 4, blockTextureFace(blockType, 4));
            vertices[vertexIndex+4] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 1, 4, blockTextureFace(blockType, 4));
            vertices[vertexIndex+5] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 4, blockTextureFace(blockType, 4));
            vertexIndex += 6;
        }

        // BOTTOM
        U32 bottomIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY] - 1, blockPos[VZ]);
        if (
            (bottomIndex >= BLOCK_COUNT && isBlockTransparent(worldGetBlock(chunk->position[VX] + blockPos[VX], chunk->position[VY] + blockPos[VY] - 1, chunk->position[VZ] + blockPos[VZ]))) || 
            (bottomIndex < BLOCK_COUNT && isBlockTransparent(chunk->blocks[bottomIndex]))
        ) {
            vertices[vertexIndex] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 0, 1, 5, blockTextureFace(blockType, 5));
            vertices[vertexIndex+1] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 1, 1, 5, blockTextureFace(blockType, 5));
            vertices[vertexIndex+2] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 0, 5, blockTextureFace(blockType, 5));

            vertices[vertexIndex+3] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 0, 5, blockTextureFace(blockType, 5));
            vertices[vertexIndex+4] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 0, 5, blockTextureFace(blockType, 5));
            vertices[vertexIndex+5] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 0, 1, 5, blockTextureFace(blockType, 5));
            vertexIndex += 6;
        }
    }

    chunk->meshVertexCount = vertexIndex;
    return vertices;
}

CHUNK* createChunk(I32* position, U8* blocks) {
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

void renderChunk(CHUNK* chunk) {
    if (chunk == NULL) return;
    if (!chunk->vertexCount) return;
    glBindVertexArray(chunk->vaoID);
    glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
}

void destroyChunk(CHUNK* chunk) {
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
