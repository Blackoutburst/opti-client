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

int packVertexData(char x, char y, char z, char u, char v, char n) {
    return (x & 31) | (y & 31) << 5 | (z & 31) << 10 | (u & 31) << 15 | (v & 31) << 20 | (n & 7) << 25;
}

void generateChunkVAO(CHUNK* chunk, CHUNK_MESH* mesh) {
    if (!mesh->vertexCount || !mesh->indexCount) return;
    glBindVertexArray(chunk->vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * mesh->vertexCount, mesh->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_INT, 4, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mesh->indexCount, mesh->indices, GL_STATIC_DRAW);
}

void cleanChunkMesh(CHUNK_MESH* mesh) {
    if (mesh->vertexCount)
        free(mesh->vertices);
    if (mesh->indexCount)
        free(mesh->indices);
    free(mesh);
}

CHUNK_MESH* generateChunkMesh(CHUNK* chunk) {
    int* vertices = malloc(sizeof(int) * BLOCK_COUNT * 24);
    int* indices = malloc(sizeof(int) * BLOCK_COUNT * 36);
    char* blockPos = malloc(sizeof(char) * 3);

    CHUNK_MESH* mesh = malloc(sizeof(CHUNK_MESH));
    mesh->vertices = vertices;
    mesh->indices = indices;

    int vertexIndex = 0;
    int indexIndex = 0;
    int indexOffset = 0;

    for (unsigned short i = BLOCK_COUNT; i; i--) {
        indexToXYZ(blockPos, i);

        // TOP
        unsigned int topIndex = xyzToIndex(blockPos[VX], blockPos[VY] + 1, blockPos[VZ]);
        if (topIndex < 4096 && !chunk->blocks[topIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 0);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 0);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 1, 0);
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 1, 0);

            indices[indexIndex++] = 0 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;
            indices[indexIndex++] = 1 + indexOffset;
            indices[indexIndex++] = 0 + indexOffset;
            indices[indexIndex++] = 3 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;

            indexOffset += 4;
        }

        // FRONT
        unsigned int frontIndex = xyzToIndex(blockPos[VX], blockPos[VY], blockPos[VZ] - 1);
        if (frontIndex < 4096 && !chunk->blocks[frontIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 1);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 1);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 1);
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 1);

            indices[indexIndex++] = 0 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;
            indices[indexIndex++] = 1 + indexOffset;
            indices[indexIndex++] = 0 + indexOffset;
            indices[indexIndex++] = 3 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;

            indexOffset += 4;
        }

        // BACK
        unsigned int backIndex = xyzToIndex(blockPos[VX], blockPos[VY], blockPos[VZ] + 1);
        if (backIndex < 4096 && !chunk->blocks[backIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 2);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 1, 2);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 2);
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 2);

            indices[indexIndex++] = 1 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;
            indices[indexIndex++] = 0 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;
            indices[indexIndex++] = 3 + indexOffset;
            indices[indexIndex++] = 0 + indexOffset;

            indexOffset += 4;
        }

        // LEFT
        unsigned int leftIndex = xyzToIndex(blockPos[VX] - 1, blockPos[VY], blockPos[VZ]);
        if (leftIndex < 4096 && !chunk->blocks[leftIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 1, 1, 3);
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ]    , 1, 0, 3);
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY] + 1, blockPos[VZ] + 1, 0, 0, 3);
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 1, 3);

            indices[indexIndex++] = 0 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;
            indices[indexIndex++] = 1 + indexOffset;
            indices[indexIndex++] = 0 + indexOffset;
            indices[indexIndex++] = 3 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;

            indexOffset += 4;
        }

        // RIGHT
        unsigned int rightIndex = xyzToIndex(blockPos[VX] + 1, blockPos[VY], blockPos[VZ]);
        if (rightIndex < 4096 && !chunk->blocks[rightIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 0, 1, 4);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ]    , 0, 0, 4);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY] + 1, blockPos[VZ] + 1, 1, 0, 4);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 1, 4);

            indices[indexIndex++] = 1 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;
            indices[indexIndex++] = 0 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;
            indices[indexIndex++] = 3 + indexOffset;
            indices[indexIndex++] = 0 + indexOffset;

            indexOffset += 4;
        }

        // BOTTOM
        unsigned int bottomIndex = xyzToIndex(blockPos[VX], blockPos[VY] - 1, blockPos[VZ]);
        if (bottomIndex < 4096 && !chunk->blocks[bottomIndex]) {
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ]    , 0, 1, 5);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ]    , 1, 1, 5);
            vertices[vertexIndex++] = packVertexData(blockPos[VX] + 1, blockPos[VY]    , blockPos[VZ] + 1, 1, 0, 5);
            vertices[vertexIndex++] = packVertexData(blockPos[VX]    , blockPos[VY]    , blockPos[VZ] + 1, 0, 0, 5);

            indices[indexIndex++] = 1 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;
            indices[indexIndex++] = 0 + indexOffset;
            indices[indexIndex++] = 2 + indexOffset;
            indices[indexIndex++] = 3 + indexOffset;
            indices[indexIndex++] = 0 + indexOffset;

            indexOffset += 4;
        }
    }

    if (vertexIndex != BLOCK_COUNT * 24)
        vertices = realloc(vertices, sizeof(int) * vertexIndex);
    if (indexIndex != BLOCK_COUNT * 36)
        indices = realloc(indices, sizeof(int) * indexIndex);

    mesh->vertexCount = vertexIndex;
    mesh->indexCount = indexIndex;
    free(blockPos);

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

void renderChunk(CHUNK* chunk) {
    if (!chunk->indexCount) return;
    glDrawElements(GL_TRIANGLES, chunk->indexCount, GL_UNSIGNED_INT, 0);
}

void destroyChunk(CHUNK* chunk) {
    glDeleteVertexArrays(1, &chunk->vaoID);
    glDeleteBuffers(1, &chunk->vboID);
    glDeleteBuffers(1, &chunk->eboID);
    free(chunk->position);
    free(chunk->blocks);
    free(chunk);
}
