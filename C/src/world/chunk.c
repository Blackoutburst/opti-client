#include <stdio.h>
#include <stdlib.h>
#include "world/chunk.h"
#include "world/world.h"
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
    printf("vao: %i, vbo: %i, ebo: %i, index count: %u position: %p blocks: %p\n", chunk->vaoID, chunk->vboID, chunk->eboID, chunk->indexCount, chunk->position, chunk->blocks);
}

int packVertexData(int x, int y, int z, int u, int v, int n) {
    return (x & 31) | (y & 31) << 5 | (z & 31) << 10 | (u & 31) << 15 | (v & 31) << 20 | (n & 7) << 25;
}

void generateChunkVAO(CHUNK * chunk, int** mesh) {
    glBindVertexArray(chunk->vaoID);

    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * BLOCK_COUNT * 24, mesh[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_INT, 4, 0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * BLOCK_COUNT * 36, mesh[1], GL_STATIC_DRAW);
}

void cleanChunkMesh(int** mesh) {
    free(mesh[0]);
    free(mesh[1]);
    free(mesh);
}

int** generateChunkMesh(CHUNK* chunk) {
    int* vertices = malloc(sizeof(int) * BLOCK_COUNT * 24);
    int* indices = malloc(sizeof(int) * BLOCK_COUNT * 36);
    int** mesh = malloc(sizeof(int*) * 2);
    mesh[0] = vertices;
    mesh[1] = indices;

    int vertexIndex = 0;
    int indexIndex = 0;
    int indexOffset = 0;
    for (unsigned long i = 0; i < BLOCK_COUNT; i++) {
        // TOP
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY] + 1, chunk->position[VZ]    , 0, 0, 0);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY] + 1, chunk->position[VZ]    , 1, 0, 0);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY] + 1, chunk->position[VZ] + 1, 1, 1, 0);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY] + 1, chunk->position[VZ] + 1, 0, 1, 0);

        indices[indexIndex++] = 0 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;
        indices[indexIndex++] = 1 + indexOffset;
        indices[indexIndex++] = 0 + indexOffset;
        indices[indexIndex++] = 3 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;

        indexOffset += 4;

        // FRONT
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY]    , chunk->position[VZ]    , 1, 1, 1);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY]    , chunk->position[VZ]    , 0, 1, 1);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY] + 1, chunk->position[VZ]    , 0, 0, 1);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY] + 1, chunk->position[VZ]    , 1, 0, 1);

        indices[indexIndex++] = 0 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;
        indices[indexIndex++] = 1 + indexOffset;
        indices[indexIndex++] = 0 + indexOffset;
        indices[indexIndex++] = 3 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;

        indexOffset += 4;

        // BACK
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY]    , chunk->position[VZ] + 1, 0, 1, 2);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY]    , chunk->position[VZ] + 1, 1, 1, 2);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY] + 1, chunk->position[VZ] + 1, 1, 0, 2);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY] + 1, chunk->position[VZ] + 1, 0, 0, 2);

        indices[indexIndex++] = 1 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;
        indices[indexIndex++] = 0 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;
        indices[indexIndex++] = 3 + indexOffset;
        indices[indexIndex++] = 0 + indexOffset;

        indexOffset += 4;

        // LEFT
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY]    , chunk->position[VZ]    , 1, 1, 3);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY] + 1, chunk->position[VZ]    , 1, 0, 3);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY] + 1, chunk->position[VZ] + 1, 0, 0, 3);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY]    , chunk->position[VZ] + 1, 0, 1, 3);

        indices[indexIndex++] = 0 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;
        indices[indexIndex++] = 1 + indexOffset;
        indices[indexIndex++] = 0 + indexOffset;
        indices[indexIndex++] = 3 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;

        indexOffset += 4;

        // RIGHT
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY]    , chunk->position[VZ]    , 0, 1, 4);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY] + 1, chunk->position[VZ]    , 0, 0, 4);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY] + 1, chunk->position[VZ] + 1, 1, 0, 4);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY]    , chunk->position[VZ] + 1, 1, 1, 4);

        indices[indexIndex++] = 1 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;
        indices[indexIndex++] = 0 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;
        indices[indexIndex++] = 3 + indexOffset;
        indices[indexIndex++] = 0 + indexOffset;

        indexOffset += 4;

        // BOTTOM
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY]    , chunk->position[VZ]    , 0, 1, 5);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY]    , chunk->position[VZ]    , 1, 1, 5);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX] + 1, chunk->position[VY]    , chunk->position[VZ] + 1, 1, 0, 5);
        vertices[vertexIndex++] = packVertexData(chunk->position[VX]    , chunk->position[VY]    , chunk->position[VZ] + 1, 0, 0, 5);

        indices[indexIndex++] = 1 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;
        indices[indexIndex++] = 0 + indexOffset;
        indices[indexIndex++] = 2 + indexOffset;
        indices[indexIndex++] = 3 + indexOffset;
        indices[indexIndex++] = 0 + indexOffset;

        indexOffset += 4;
    }

    return mesh;
}

CHUNK* createChunk(int* position, char* blocks) {
    CHUNK* chunk = malloc(sizeof(CHUNK));

    glGenVertexArrays(1, &chunk->vaoID);
    glGenBuffers(1, &chunk->vboID);
    glGenBuffers(1, &chunk->eboID);

    chunk->indexCount = BLOCK_COUNT * 36;
    chunk->position = position;
    chunk->blocks = blocks;

    return chunk;
}

void destroyChunk(CHUNK* chunk) {
    glDeleteVertexArrays(1, &chunk->vaoID);
    glDeleteBuffers(1, &chunk->vboID);
    glDeleteBuffers(1, &chunk->eboID);
    free(chunk->position);
    free(chunk->blocks);
    free(chunk);
}
