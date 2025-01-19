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

static I32* verticesMemorySpace = NULL;
static I8* blockPosMemorySpace = NULL;

void initChunkMemoryRegion() {
    verticesMemorySpace = malloc(sizeof(I32) * BLOCK_COUNT * 36);
    blockPosMemorySpace = malloc(sizeof(I8) * 3);
}

void cleanChunkMemoryRegion() {
    free(verticesMemorySpace);
    free(blockPosMemorySpace);
    verticesMemorySpace = NULL;
    blockPosMemorySpace = NULL;
}

void printChunk(CHUNK* chunk) {
    printf("vao: %i, vbo: %i, ebo: %i, vertex count: %u position: %p blocks: %p\n", chunk->vaoID, chunk->vboID, chunk->eboID, chunk->vertexCount, chunk->position, chunk->blocks);
}

I32 packVertexData(I8 x, I8 y, I8 z, I8 u, I8 v, I8 n, I8 t) {
    return (x & 31) | (y & 31) << 5 | (z & 31) << 10 | (u & 31) << 15 | (v & 31) << 20 | (n & 7) << 25 | (t & 15) << 28;
}

void generateChunkVAO(CHUNK* chunk, I32* vertices) {
    if (!chunk->vertexCount) return;
    glBindVertexArray(chunk->vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(I32) * chunk->vertexCount, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_INT, 4, 0);
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

I32* generateChunkMesh(CHUNK* chunk, U8 isMonotype) {
    if (isMonotype) {
        if (!chunk->blocks[0]) return NULL;
        I32 blockType = chunk->blocks[0];

        // TOP
        verticesMemorySpace[ 0] = packVertexData(CHUNK_SIZE, CHUNK_SIZE,          0, CHUNK_SIZE,          0, 0, blockTextureFace(blockType, 0));
        verticesMemorySpace[ 1] = packVertexData(         0, CHUNK_SIZE,          0,          0,          0, 0, blockTextureFace(blockType, 0));
        verticesMemorySpace[ 2] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 0, blockTextureFace(blockType, 0));

        verticesMemorySpace[ 3] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 0, blockTextureFace(blockType, 0));
        verticesMemorySpace[ 4] = packVertexData(         0, CHUNK_SIZE,          0,          0,          0, 0, blockTextureFace(blockType, 0));
        verticesMemorySpace[ 5] = packVertexData(         0, CHUNK_SIZE, CHUNK_SIZE,          0, CHUNK_SIZE, 0, blockTextureFace(blockType, 0));

        // FRONT
        verticesMemorySpace[ 6] = packVertexData(CHUNK_SIZE,          0,          0,          0, CHUNK_SIZE, 1, blockTextureFace(blockType, 1));
        verticesMemorySpace[ 7] = packVertexData(         0,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 1, blockTextureFace(blockType, 1));
        verticesMemorySpace[ 8] = packVertexData(CHUNK_SIZE, CHUNK_SIZE,          0,          0,          0, 1, blockTextureFace(blockType, 1));

        verticesMemorySpace[ 9] = packVertexData(CHUNK_SIZE, CHUNK_SIZE,          0,          0,          0, 1, blockTextureFace(blockType, 1));
        verticesMemorySpace[10] = packVertexData(         0,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 1, blockTextureFace(blockType, 1));
        verticesMemorySpace[11] = packVertexData(         0, CHUNK_SIZE,          0, CHUNK_SIZE,          0, 1, blockTextureFace(blockType, 1));

        // BACK
        verticesMemorySpace[12] = packVertexData(         0,          0, CHUNK_SIZE,          0, CHUNK_SIZE, 2, blockTextureFace(blockType, 2));
        verticesMemorySpace[13] = packVertexData(CHUNK_SIZE,          0, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 2, blockTextureFace(blockType, 2));
        verticesMemorySpace[14] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,          0, 2, blockTextureFace(blockType, 2));

        verticesMemorySpace[15] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,          0, 2, blockTextureFace(blockType, 2));
        verticesMemorySpace[16] = packVertexData(         0, CHUNK_SIZE, CHUNK_SIZE,          0,          0, 2, blockTextureFace(blockType, 2));
        verticesMemorySpace[17] = packVertexData(         0,          0, CHUNK_SIZE,          0, CHUNK_SIZE, 2, blockTextureFace(blockType, 2));

        // LEFT
        verticesMemorySpace[18] = packVertexData(         0, CHUNK_SIZE, CHUNK_SIZE,          0,          0, 3, blockTextureFace(blockType, 3));
        verticesMemorySpace[19] = packVertexData(         0, CHUNK_SIZE,          0, CHUNK_SIZE,          0, 3, blockTextureFace(blockType, 3));
        verticesMemorySpace[20] = packVertexData(         0,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 3, blockTextureFace(blockType, 3));

        verticesMemorySpace[21] = packVertexData(         0,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 3, blockTextureFace(blockType, 3));
        verticesMemorySpace[22] = packVertexData(         0,          0, CHUNK_SIZE,          0, CHUNK_SIZE, 3, blockTextureFace(blockType, 3));
        verticesMemorySpace[23] = packVertexData(         0, CHUNK_SIZE, CHUNK_SIZE,          0,          0, 3, blockTextureFace(blockType, 3));

        // RIGHT
        verticesMemorySpace[24] = packVertexData(CHUNK_SIZE,          0,          0,          0, CHUNK_SIZE, 4, blockTextureFace(blockType, 4));
        verticesMemorySpace[25] = packVertexData(CHUNK_SIZE, CHUNK_SIZE,          0,          0,          0, 4, blockTextureFace(blockType, 4));
        verticesMemorySpace[26] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,          0, 4, blockTextureFace(blockType, 4));

        verticesMemorySpace[27] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,          0, 4, blockTextureFace(blockType, 4));
        verticesMemorySpace[28] = packVertexData(CHUNK_SIZE,          0, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 4, blockTextureFace(blockType, 4));
        verticesMemorySpace[29] = packVertexData(CHUNK_SIZE,          0,          0,          0, CHUNK_SIZE, 4, blockTextureFace(blockType, 4));

        // BOTTOM
        verticesMemorySpace[30] = packVertexData(         0,          0,          0,          0, CHUNK_SIZE, 5, blockTextureFace(blockType, 5));
        verticesMemorySpace[31] = packVertexData(CHUNK_SIZE,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 5, blockTextureFace(blockType, 5));
        verticesMemorySpace[32] = packVertexData(CHUNK_SIZE,          0, CHUNK_SIZE, CHUNK_SIZE,          0, 5, blockTextureFace(blockType, 5));

        verticesMemorySpace[33] = packVertexData(CHUNK_SIZE,          0, CHUNK_SIZE, CHUNK_SIZE,          0, 5, blockTextureFace(blockType, 5));
        verticesMemorySpace[34] = packVertexData(         0,          0, CHUNK_SIZE,          0,          0, 5, blockTextureFace(blockType, 5));
        verticesMemorySpace[35] = packVertexData(         0,          0,          0,          0, CHUNK_SIZE, 5, blockTextureFace(blockType, 5));

        chunk->vertexCount = 36;
        return verticesMemorySpace;
    }

    I32 vertexIndex = 0;
    for (U16 i = 0; i < BLOCK_COUNT; i++) {
        I32 blockType = chunk->blocks[i];
        if (!blockType) continue;
        indexToXYZ(blockPosMemorySpace, i);

        // TOP
        U32 topIndex = xyzToIndexOobCheck(blockPosMemorySpace[VX], blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ]);
        if (topIndex >= BLOCK_COUNT || isBlockTransparent(chunk->blocks[topIndex])) {
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ]    , 1, 0, 0, blockTextureFace(blockType, 0));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ]    , 0, 0, 0, blockTextureFace(blockType, 0));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 1, 1, 0, blockTextureFace(blockType, 0));

            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 1, 1, 0, blockTextureFace(blockType, 0));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ]    , 0, 0, 0, blockTextureFace(blockType, 0));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 0, 1, 0, blockTextureFace(blockType, 0));
        }

        // FRONT
        U32 frontIndex = xyzToIndexOobCheck(blockPosMemorySpace[VX], blockPosMemorySpace[VY], blockPosMemorySpace[VZ] - 1);
        if (frontIndex > BLOCK_COUNT || isBlockTransparent(chunk->blocks[frontIndex])) {
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 0, 1, 1, blockTextureFace(blockType, 1));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 1, 1, 1, blockTextureFace(blockType, 1));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ]    , 0, 0, 1, blockTextureFace(blockType, 1));

            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ]    , 0, 0, 1, blockTextureFace(blockType, 1));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 1, 1, 1, blockTextureFace(blockType, 1));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ]    , 1, 0, 1, blockTextureFace(blockType, 1));
        }

        // BACK
        U32 backIndex = xyzToIndexOobCheck(blockPosMemorySpace[VX], blockPosMemorySpace[VY], blockPosMemorySpace[VZ] + 1);
        if (backIndex >= BLOCK_COUNT || isBlockTransparent(chunk->blocks[backIndex])) {
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ] + 1, 0, 1, 2, blockTextureFace(blockType, 2));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ] + 1, 1, 1, 2, blockTextureFace(blockType, 2));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 1, 0, 2, blockTextureFace(blockType, 2));

            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 1, 0, 2, blockTextureFace(blockType, 2));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 0, 0, 2, blockTextureFace(blockType, 2));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ] + 1, 0, 1, 2, blockTextureFace(blockType, 2));
        }

        // LEFT
        U32 leftIndex = xyzToIndexOobCheck(blockPosMemorySpace[VX] - 1, blockPosMemorySpace[VY], blockPosMemorySpace[VZ]);
        if (leftIndex >= BLOCK_COUNT || isBlockTransparent(chunk->blocks[leftIndex])) {
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 0, 0, 3, blockTextureFace(blockType, 3));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ]    , 1, 0, 3, blockTextureFace(blockType, 3));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 1, 1, 3, blockTextureFace(blockType, 3));

            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 1, 1, 3, blockTextureFace(blockType, 3));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ] + 1, 0, 1, 3, blockTextureFace(blockType, 3));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 0, 0, 3, blockTextureFace(blockType, 3));
        }

        // RIGHT
        U32 rightIndex = xyzToIndexOobCheck(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY], blockPosMemorySpace[VZ]);
        if (rightIndex >= BLOCK_COUNT || isBlockTransparent(chunk->blocks[rightIndex])) {
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 0, 1, 4, blockTextureFace(blockType, 4));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ]    , 0, 0, 4, blockTextureFace(blockType, 4));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 1, 0, 4, blockTextureFace(blockType, 4));

            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY] + 1, blockPosMemorySpace[VZ] + 1, 1, 0, 4, blockTextureFace(blockType, 4));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ] + 1, 1, 1, 4, blockTextureFace(blockType, 4));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 0, 1, 4, blockTextureFace(blockType, 4));
        }

        // BOTTOM
        U32 bottomIndex = xyzToIndexOobCheck(blockPosMemorySpace[VX], blockPosMemorySpace[VY] - 1, blockPosMemorySpace[VZ]);
        if (bottomIndex >= BLOCK_COUNT || isBlockTransparent(chunk->blocks[bottomIndex])) {
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 0, 1, 5, blockTextureFace(blockType, 5));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 1, 1, 5, blockTextureFace(blockType, 5));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ] + 1, 1, 0, 5, blockTextureFace(blockType, 5));

            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX] + 1, blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ] + 1, 1, 0, 5, blockTextureFace(blockType, 5));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ] + 1, 0, 0, 5, blockTextureFace(blockType, 5));
            verticesMemorySpace[vertexIndex++] = packVertexData(blockPosMemorySpace[VX]    , blockPosMemorySpace[VY]    , blockPosMemorySpace[VZ]    , 0, 1, 5, blockTextureFace(blockType, 5));
        }
    }

    chunk->vertexCount = vertexIndex;
    return verticesMemorySpace;
}

CHUNK* createChunk(I32* position, U8* blocks) {
    CHUNK* chunk = malloc(sizeof(CHUNK));

    glGenVertexArrays(1, &chunk->vaoID);
    glGenBuffers(1, &chunk->vboID);
    glGenBuffers(1, &chunk->eboID);

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
