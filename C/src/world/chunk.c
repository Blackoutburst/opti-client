#include <stdio.h>
#include <stdlib.h>
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

int packVertexData(char x, char y, char z, char u, char v, char n, char t) {
    return (x & 31) | (y & 31) << 5 | (z & 31) << 10 | (u & 31) << 15 | (v & 31) << 20 | (n & 7) << 25 | (t & 15) << 28;
}

void generateChunkVAO(CHUNK* chunk, int* vertices) {
    if (!chunk->vertexCount) return;
    glBindVertexArray(chunk->vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * chunk->vertexCount, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_INT, 4, 0);
}

void cleanChunkMesh(int* vertices) {
    free(vertices);
}

char isChunkMonotype(CHUNK* chunk) {
    int previous = chunk->blocks[0];
    for (int i = BLOCK_COUNT - 1; i; i--) {
        if (previous != chunk->blocks[i])
            return 0;
        previous = chunk->blocks[i];
    }

    return 1;
}

int blockTextureFace(int blockType, int face) {
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
        default: return ERROR;
    }
}

int* generateChunkMesh(CHUNK* chunk) {
    if (isChunkMonotype(chunk)) {
        int blockType = chunk->blocks[0];
        int* vertices = malloc(sizeof(int) * 36);

        // TOP
        vertices[ 0] = packVertexData(CHUNK_SIZE, CHUNK_SIZE,          0, CHUNK_SIZE,          0, 0, blockTextureFace(blockType, 0));
        vertices[ 1] = packVertexData(         0, CHUNK_SIZE,          0,          0,          0, 0, blockTextureFace(blockType, 0));
        vertices[ 2] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 0, blockTextureFace(blockType, 0));
        
        vertices[ 3] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 0, blockTextureFace(blockType, 0));
        vertices[ 4] = packVertexData(         0, CHUNK_SIZE,          0,          0,          0, 0, blockTextureFace(blockType, 0));
        vertices[ 5] = packVertexData(         0, CHUNK_SIZE, CHUNK_SIZE,          0, CHUNK_SIZE, 0, blockTextureFace(blockType, 0));

        // FRONT
        vertices[ 6] = packVertexData(CHUNK_SIZE,          0,          0,          0, CHUNK_SIZE, 1, blockTextureFace(blockType, 1));
        vertices[ 7] = packVertexData(         0,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 1, blockTextureFace(blockType, 1));
        vertices[ 8] = packVertexData(CHUNK_SIZE, CHUNK_SIZE,          0,          0,          0, 1, blockTextureFace(blockType, 1));
        
        vertices[ 9] = packVertexData(CHUNK_SIZE, CHUNK_SIZE,          0,          0,          0, 1, blockTextureFace(blockType, 1));
        vertices[10] = packVertexData(         0,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 1, blockTextureFace(blockType, 1));
        vertices[11] = packVertexData(         0, CHUNK_SIZE,          0, CHUNK_SIZE,          0, 1, blockTextureFace(blockType, 1));

        // BACK
        vertices[12] = packVertexData(         0,          0, CHUNK_SIZE,          0, CHUNK_SIZE, 2, blockTextureFace(blockType, 2));
        vertices[13] = packVertexData(CHUNK_SIZE,          0, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 2, blockTextureFace(blockType, 2));
        vertices[14] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,          0, 2, blockTextureFace(blockType, 2));
        
        vertices[15] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,          0, 2, blockTextureFace(blockType, 2));
        vertices[16] = packVertexData(         0, CHUNK_SIZE, CHUNK_SIZE,          0,          0, 2, blockTextureFace(blockType, 2));
        vertices[17] = packVertexData(         0,          0, CHUNK_SIZE,          0, CHUNK_SIZE, 2, blockTextureFace(blockType, 2));

        // LEFT
        vertices[18] = packVertexData(         0, CHUNK_SIZE, CHUNK_SIZE,          0,          0, 3, blockTextureFace(blockType, 3));
        vertices[19] = packVertexData(         0, CHUNK_SIZE,          0, CHUNK_SIZE,          0, 3, blockTextureFace(blockType, 3));
        vertices[20] = packVertexData(         0,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 3, blockTextureFace(blockType, 3));
        
        vertices[21] = packVertexData(         0,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 3, blockTextureFace(blockType, 3));
        vertices[22] = packVertexData(         0,          0, CHUNK_SIZE,          0, CHUNK_SIZE, 3, blockTextureFace(blockType, 3));
        vertices[23] = packVertexData(         0, CHUNK_SIZE, CHUNK_SIZE,          0,          0, 3, blockTextureFace(blockType, 3));

        // RIGHT
        vertices[24] = packVertexData(CHUNK_SIZE,          0,          0,          0, CHUNK_SIZE, 4, blockTextureFace(blockType, 4));
        vertices[25] = packVertexData(CHUNK_SIZE, CHUNK_SIZE,          0,          0,          0, 4, blockTextureFace(blockType, 4));
        vertices[26] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,          0, 4, blockTextureFace(blockType, 4));
        
        vertices[27] = packVertexData(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,          0, 4, blockTextureFace(blockType, 4));
        vertices[28] = packVertexData(CHUNK_SIZE,          0, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 4, blockTextureFace(blockType, 4));
        vertices[29] = packVertexData(CHUNK_SIZE,          0,          0,          0, CHUNK_SIZE, 4, blockTextureFace(blockType, 4));

        // BOTTOM
        vertices[30] = packVertexData(         0,          0,          0,          0, CHUNK_SIZE, 5, blockTextureFace(blockType, 5));
        vertices[31] = packVertexData(CHUNK_SIZE,          0,          0, CHUNK_SIZE, CHUNK_SIZE, 5, blockTextureFace(blockType, 5));
        vertices[32] = packVertexData(CHUNK_SIZE,          0, CHUNK_SIZE, CHUNK_SIZE,          0, 5, blockTextureFace(blockType, 5));
        
        vertices[33] = packVertexData(CHUNK_SIZE,          0, CHUNK_SIZE, CHUNK_SIZE,          0, 5, blockTextureFace(blockType, 5));
        vertices[34] = packVertexData(         0,          0, CHUNK_SIZE,          0,          0, 5, blockTextureFace(blockType, 5));
        vertices[35] = packVertexData(         0,          0,          0,          0, CHUNK_SIZE, 5, blockTextureFace(blockType, 5));

        chunk->vertexCount = 36;
        return vertices;
    }

    // TODO: Take out use static malloc and reus each time we generate a chunk
    int* vertices = malloc(sizeof(int) * BLOCK_COUNT * 36);
    char* blockPos = malloc(sizeof(char) * 3);

    int vertexIndex = 0;
    for (unsigned short i = BLOCK_COUNT; i; i--) {
        int blockType = chunk->blocks[i];
        indexToXYZ(blockPos, i);

        // TOP
        unsigned int topIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY] + 1, blockPos[VZ]);
        if (topIndex >= BLOCK_COUNT || !chunk->blocks[topIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 0, blockTextureFace(blockType, 0));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 0, blockTextureFace(blockType, 0));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 1, 0, blockTextureFace(blockType, 0));
            
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 1, 0, blockTextureFace(blockType, 0));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 0, blockTextureFace(blockType, 0));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 1, 0, blockTextureFace(blockType, 0));
        }

        // FRONT
        unsigned int frontIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY], blockPos[VZ] - 1);
        if (frontIndex > BLOCK_COUNT || !chunk->blocks[frontIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 1, blockTextureFace(blockType, 1));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 1, blockTextureFace(blockType, 1));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 1, blockTextureFace(blockType, 1));
            
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 1, blockTextureFace(blockType, 1));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 1, blockTextureFace(blockType, 1));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 1, blockTextureFace(blockType, 1));
        }

        // BACK
        unsigned int backIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY], blockPos[VZ] + 1);
        if (backIndex >= BLOCK_COUNT || !chunk->blocks[backIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 2, blockTextureFace(blockType, 2));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 1, 2, blockTextureFace(blockType, 2));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 2, blockTextureFace(blockType, 2));
            
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 2, blockTextureFace(blockType, 2));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 2, blockTextureFace(blockType, 2));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 2, blockTextureFace(blockType, 2));
        }

        // LEFT
        unsigned int leftIndex = xyzToIndexOobCheck(blockPos[VX] - 1, blockPos[VY], blockPos[VZ]);
        if (leftIndex >= BLOCK_COUNT || !chunk->blocks[leftIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 3, blockTextureFace(blockType, 3));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 3, blockTextureFace(blockType, 3));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 3, blockTextureFace(blockType, 3));
            
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 3, blockTextureFace(blockType, 3));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 3, blockTextureFace(blockType, 3));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 3, blockTextureFace(blockType, 3));
        }

        // RIGHT
        unsigned int rightIndex = xyzToIndexOobCheck(blockPos[VX] + 1, blockPos[VY], blockPos[VZ]);
        if (rightIndex >= BLOCK_COUNT || !chunk->blocks[rightIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 4, blockTextureFace(blockType, 4));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 4, blockTextureFace(blockType, 4));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 4, blockTextureFace(blockType, 4));
            
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 4, blockTextureFace(blockType, 4));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 1, 4, blockTextureFace(blockType, 4));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 4, blockTextureFace(blockType, 4));
        }

        // BOTTOM
        unsigned int bottomIndex = xyzToIndexOobCheck(blockPos[VX], blockPos[VY] - 1, blockPos[VZ]);
        if (bottomIndex >= BLOCK_COUNT || !chunk->blocks[bottomIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 0, 1, 5, blockTextureFace(blockType, 5));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 1, 1, 5, blockTextureFace(blockType, 5));
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 0, 5, blockTextureFace(blockType, 5));
            
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 0, 5, blockTextureFace(blockType, 5));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 0, 5, blockTextureFace(blockType, 5));
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 0, 1, 5, blockTextureFace(blockType, 5));
        }
    }

    free(blockPos);

    chunk->vertexCount = vertexIndex;

    return vertices;
}

CHUNK* createChunk(int* position, char* blocks) {
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
    if (!chunk->vertexCount) return;
    glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
}

void destroyChunk(CHUNK* chunk) {
    glDeleteVertexArrays(1, &chunk->vaoID);
    glDeleteBuffers(1, &chunk->vboID);
    glDeleteBuffers(1, &chunk->eboID);
    free(chunk->position);
    free(chunk->blocks);
    free(chunk);
}
