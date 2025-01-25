#include <stdlib.h>
#include <stdio.h>
#include "graphics/opengl.h"
#include "utils/types.h"
#include "utils/math.h"
#include "world/blocks.h"
#include "world/world.h"
#include "world/chunk.h"
#include "world/meshQueue.h"

void chunkPrint(CHUNK* chunk) {
    printf("vao: %i, vbo: %i, ebo: %i, vertex count: %u position: %p blocks: %p\n", chunk->vaoID, chunk->vboID, chunk->eboID, chunk->indexCount, chunk->position, chunk->blocks);
}

I32 chunkPackVertexData(I8 x, I8 y, I8 z, I8 u, I8 v, I8 n, I8 t) {
    return (x & 31) | (y & 31) << 5 | (z & 31) << 10 | (u & 3) << 15 | (v & 3) << 17 | (n & 7) << 19 | (t & 31) << 23;
}

void chunkGenerateVAO(CHUNK* chunk, CHUNK_MESH* mesh) {
    if (!mesh->indexCount) {
        free(mesh->vertices);
        free(mesh->indices);
        free(mesh);
        return;
    }
    
    glBindVertexArray(chunk->vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(I32) * mesh->vertexCount, mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(I32) * mesh->indexCount, mesh->indices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_INT, 4, 0);
    
    chunk->indexCount = mesh->indexCount;
    
    free(mesh->vertices);
    free(mesh->indices);
    free(mesh);

}



U8 _chunkIsMonotype(U8* blocks) {
    I32 previous = blocks[0];
    
    for (I32 i = BLOCK_COUNT - 1; i; i--) {
        if (previous != blocks[i])
            return 0;
        previous = blocks[i];
    }

    return 1;
}

U8 chunkIsMonotype(CHUNK* chunk) {
    return _chunkIsMonotype(chunk->blocks);
}

CHUNK_MESH* chunkGenerateMesh(VECTORI* position, U8* blocks) {
    CHUNK_MESH* mesh = malloc(sizeof(CHUNK_MESH));
    I32* vertices = malloc(sizeof(I32) * BLOCK_COUNT * 24);
    I32* indices = malloc(sizeof(I32) * BLOCK_COUNT * 36);
    I32 indexIndex = 0;
    I32 indexCount = 0;

    //EMPTY
    if (_chunkIsMonotype(blocks) && blocks[0] == 0) {
        mesh->vertices = vertices;
        mesh->indices = indices;
        mesh->vertexCount = indexIndex;
        mesh->indexCount = indexCount;
        return mesh;
    }
    
    // MONO

    if (_chunkIsMonotype(blocks)) {
        I32 blockType = blocks[0];
        //TOP BOTTOM
        U8 top = 0;
        U8 bottom = 0;
        for (U8 x = 0; x < CHUNK_SIZE; x++) {
        for (U8 z = 0; z < CHUNK_SIZE; z++) {
            if (blocksIsTransparent(worldGetBlock(position->x + x, position->y + CHUNK_SIZE, position->z + z))) top = 1;
            if (blocksIsTransparent(worldGetBlock(position->x + x, position->y - 1, position->z + z))) bottom = 1;
        }}

        // TOP
        if (top) {
            vertices[indexIndex  ] = chunkPackVertexData(0    , 0 + CHUNK_SIZE, 0    , 0, 0, 0, blocksTextureFace(blockType, 0));
            vertices[indexIndex+1] = chunkPackVertexData(0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 0    , 2, 0, 0, blocksTextureFace(blockType, 0));
            vertices[indexIndex+2] = chunkPackVertexData(0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 2, 2, 0, blocksTextureFace(blockType, 0));
            vertices[indexIndex+3] = chunkPackVertexData(0    , 0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 0, 2, 0, blocksTextureFace(blockType, 0));
            
            indices[indexCount  ] = 0 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 1 + indexIndex;
            indices[indexCount+3] = 0 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 2 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        // BOTTOM
        if (bottom) {
            vertices[indexIndex  ] = chunkPackVertexData(0    , 0    , 0    , 0, 2, 5, blocksTextureFace(blockType, 5));
            vertices[indexIndex+1] = chunkPackVertexData(0 + CHUNK_SIZE, 0    , 0    , 2, 2, 5, blocksTextureFace(blockType, 5));
            vertices[indexIndex+2] = chunkPackVertexData(0 + CHUNK_SIZE, 0    , 0 + CHUNK_SIZE, 2, 0, 5, blocksTextureFace(blockType, 5));
            vertices[indexIndex+3] = chunkPackVertexData(0    , 0    , 0 + CHUNK_SIZE, 0, 0, 5, blocksTextureFace(blockType, 5));
            
            indices[indexCount  ] = 1 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 0 + indexIndex;
            indices[indexCount+3] = 2 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 0 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        //FRONT BACK
        U8 front = 0;
        U8 back = 0;
        for (U8 x = 0; x < CHUNK_SIZE; x++) {
        for (U8 y = 0; y < CHUNK_SIZE; y++) {
            if (blocksIsTransparent(worldGetBlock(position->x + x, position->y + y, position->z - 1))) front = 1;
            if (blocksIsTransparent(worldGetBlock(position->x + x, position->y + y, position->z + CHUNK_SIZE))) back = 1;
        }}

        // FRONT
        if (front) {
            vertices[indexIndex  ] = chunkPackVertexData(0    , 0    , 0    , 2, 2, 1, blocksTextureFace(blockType, 1));
            vertices[indexIndex+1] = chunkPackVertexData(0 + CHUNK_SIZE, 0    , 0    , 0, 2, 1, blocksTextureFace(blockType, 1));
            vertices[indexIndex+2] = chunkPackVertexData(0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 0    , 0, 0, 1, blocksTextureFace(blockType, 1));
            vertices[indexIndex+3] = chunkPackVertexData(0    , 0 + CHUNK_SIZE, 0    , 2, 0, 1, blocksTextureFace(blockType, 1));
            
            indices[indexCount  ] = 0 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 1 + indexIndex;
            indices[indexCount+3] = 0 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 2 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        // BACK
        if (back) {
            vertices[indexIndex  ] = chunkPackVertexData(0    , 0    , 0 + CHUNK_SIZE, 0, 2, 2, blocksTextureFace(blockType, 2));
            vertices[indexIndex+1] = chunkPackVertexData(0 + CHUNK_SIZE, 0    , 0 + CHUNK_SIZE, 2, 2, 2, blocksTextureFace(blockType, 2));
            vertices[indexIndex+2] = chunkPackVertexData(0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 2, 0, 2, blocksTextureFace(blockType, 2));
            vertices[indexIndex+3] = chunkPackVertexData(0    , 0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 0, 0, 2, blocksTextureFace(blockType, 2));
            
            indices[indexCount  ] = 1 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 0 + indexIndex;
            indices[indexCount+3] = 2 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 0 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        //LEFT RIGHT
        U8 left = 0;
        U8 right = 0;
        for (U8 y = 0; y < CHUNK_SIZE; y++) {
        for (U8 z = 0; z < CHUNK_SIZE; z++) {
            if (blocksIsTransparent(worldGetBlock(position->x - 1, position->y + y, position->z + z))) left = 1;
            if (blocksIsTransparent(worldGetBlock(position->x + CHUNK_SIZE, position->y + y, position->z + z))) right = 1;
        }}

        if (left) {
            vertices[indexIndex  ] = chunkPackVertexData(0    , 0    , 0    , 2, 2, 3, blocksTextureFace(blockType, 3));
            vertices[indexIndex+1] = chunkPackVertexData(0    , 0 + CHUNK_SIZE, 0    , 2, 0, 3, blocksTextureFace(blockType, 3));
            vertices[indexIndex+2] = chunkPackVertexData(0    , 0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 0, 0, 3, blocksTextureFace(blockType, 3));
            vertices[indexIndex+3] = chunkPackVertexData(0    , 0    , 0 + CHUNK_SIZE, 0, 2, 3, blocksTextureFace(blockType, 3));
            
            indices[indexCount  ] = 0 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 1 + indexIndex;
            indices[indexCount+3] = 0 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 2 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        if (right) {
            vertices[indexIndex  ] = chunkPackVertexData(0 + CHUNK_SIZE, 0    , 0    , 0, 2, 4, blocksTextureFace(blockType, 4));
            vertices[indexIndex+1] = chunkPackVertexData(0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 0    , 0, 0, 4, blocksTextureFace(blockType, 4));
            vertices[indexIndex+2] = chunkPackVertexData(0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 0 + CHUNK_SIZE, 2, 0, 4, blocksTextureFace(blockType, 4));
            vertices[indexIndex+3] = chunkPackVertexData(0 + CHUNK_SIZE, 0    , 0 + CHUNK_SIZE, 2, 2, 4, blocksTextureFace(blockType, 4));
            
            indices[indexCount  ] = 1 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 0 + indexIndex;
            indices[indexCount+3] = 2 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 0 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        mesh->vertices = vertices;
        mesh->indices = indices;
        mesh->vertexCount = indexIndex;
        mesh->indexCount = indexCount;
        return mesh;
    }

    // REGULAR

    VECTORI* blockPos = vectoriInit();
    
    for (U16 i = 0; i < BLOCK_COUNT; i++) {
        I32 blockType = blocks[i];
        if (!blockType) continue;
        
        indexToXYZ(blockPos, i);

        // TOP
        U32 topIndex = xyzToIndexOobCheck(blockPos->x, blockPos->y + 1, blockPos->z);
        if (
            (topIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(position->x + blockPos->x, position->y + blockPos->y + 1, position->z + blockPos->z))) || 
            (topIndex < BLOCK_COUNT && blocksIsTransparent(blocks[topIndex]))
        ) {
            vertices[indexIndex  ] = chunkPackVertexData(blockPos->x    , blockPos->y + 1, blockPos->z    , 0, 0, 0, blocksTextureFace(blockType, 0));
            vertices[indexIndex+1] = chunkPackVertexData(blockPos->x + 1, blockPos->y + 1, blockPos->z    , 1, 0, 0, blocksTextureFace(blockType, 0));
            vertices[indexIndex+2] = chunkPackVertexData(blockPos->x + 1, blockPos->y + 1, blockPos->z + 1, 1, 1, 0, blocksTextureFace(blockType, 0));
            vertices[indexIndex+3] = chunkPackVertexData(blockPos->x    , blockPos->y + 1, blockPos->z + 1, 0, 1, 0, blocksTextureFace(blockType, 0));
            
            indices[indexCount  ] = 0 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 1 + indexIndex;
            indices[indexCount+3] = 0 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 2 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        // FRONT
        U32 frontIndex = xyzToIndexOobCheck(blockPos->x, blockPos->y, blockPos->z - 1);
        if (
            (frontIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(position->x + blockPos->x, position->y + blockPos->y, position->z + blockPos->z - 1))) || 
            (frontIndex < BLOCK_COUNT && blocksIsTransparent(blocks[frontIndex]))
        ) {
            vertices[indexIndex  ] = chunkPackVertexData(blockPos->x    , blockPos->y    , blockPos->z    , 1, 1, 1, blocksTextureFace(blockType, 1));
            vertices[indexIndex+1] = chunkPackVertexData(blockPos->x + 1, blockPos->y    , blockPos->z    , 0, 1, 1, blocksTextureFace(blockType, 1));
            vertices[indexIndex+2] = chunkPackVertexData(blockPos->x + 1, blockPos->y + 1, blockPos->z    , 0, 0, 1, blocksTextureFace(blockType, 1));
            vertices[indexIndex+3] = chunkPackVertexData(blockPos->x    , blockPos->y + 1, blockPos->z    , 1, 0, 1, blocksTextureFace(blockType, 1));
            
            indices[indexCount  ] = 0 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 1 + indexIndex;
            indices[indexCount+3] = 0 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 2 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        // BACK
        U32 backIndex = xyzToIndexOobCheck(blockPos->x, blockPos->y, blockPos->z + 1);
        if (
            (backIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(position->x + blockPos->x, position->y + blockPos->y, position->z + blockPos->z + 1))) || 
            (backIndex < BLOCK_COUNT && blocksIsTransparent(blocks[backIndex]))
        ) {
            vertices[indexIndex  ] = chunkPackVertexData(blockPos->x    , blockPos->y    , blockPos->z + 1, 0, 1, 2, blocksTextureFace(blockType, 2));
            vertices[indexIndex+1] = chunkPackVertexData(blockPos->x + 1, blockPos->y    , blockPos->z + 1, 1, 1, 2, blocksTextureFace(blockType, 2));
            vertices[indexIndex+2] = chunkPackVertexData(blockPos->x + 1, blockPos->y + 1, blockPos->z + 1, 1, 0, 2, blocksTextureFace(blockType, 2));
            vertices[indexIndex+3] = chunkPackVertexData(blockPos->x    , blockPos->y + 1, blockPos->z + 1, 0, 0, 2, blocksTextureFace(blockType, 2));
            
            indices[indexCount  ] = 1 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 0 + indexIndex;
            indices[indexCount+3] = 2 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 0 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        // LEFT
        U32 leftIndex = xyzToIndexOobCheck(blockPos->x - 1, blockPos->y, blockPos->z);
        if (
            (leftIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(position->x + blockPos->x - 1, position->y + blockPos->y, position->z + blockPos->z))) || 
            (leftIndex < BLOCK_COUNT && blocksIsTransparent(blocks[leftIndex]))
        ) {
            vertices[indexIndex  ] = chunkPackVertexData(blockPos->x    , blockPos->y    , blockPos->z    , 1, 1, 3, blocksTextureFace(blockType, 3));
            vertices[indexIndex+1] = chunkPackVertexData(blockPos->x    , blockPos->y + 1, blockPos->z    , 1, 0, 3, blocksTextureFace(blockType, 3));
            vertices[indexIndex+2] = chunkPackVertexData(blockPos->x    , blockPos->y + 1, blockPos->z + 1, 0, 0, 3, blocksTextureFace(blockType, 3));
            vertices[indexIndex+3] = chunkPackVertexData(blockPos->x    , blockPos->y    , blockPos->z + 1, 0, 1, 3, blocksTextureFace(blockType, 3));
            
            indices[indexCount  ] = 0 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 1 + indexIndex;
            indices[indexCount+3] = 0 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 2 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        // RIGHT
        U32 rightIndex = xyzToIndexOobCheck(blockPos->x + 1, blockPos->y, blockPos->z);
        if (
            (rightIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(position->x + blockPos->x + 1, position->y + blockPos->y, position->z + blockPos->z))) || 
            (rightIndex < BLOCK_COUNT && blocksIsTransparent(blocks[rightIndex]))
        ) {
            vertices[indexIndex  ] = chunkPackVertexData(blockPos->x + 1, blockPos->y    , blockPos->z    , 0, 1, 4, blocksTextureFace(blockType, 4));
            vertices[indexIndex+1] = chunkPackVertexData(blockPos->x + 1, blockPos->y + 1, blockPos->z    , 0, 0, 4, blocksTextureFace(blockType, 4));
            vertices[indexIndex+2] = chunkPackVertexData(blockPos->x + 1, blockPos->y + 1, blockPos->z + 1, 1, 0, 4, blocksTextureFace(blockType, 4));
            vertices[indexIndex+3] = chunkPackVertexData(blockPos->x + 1, blockPos->y    , blockPos->z + 1, 1, 1, 4, blocksTextureFace(blockType, 4));
            
            indices[indexCount  ] = 1 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 0 + indexIndex;
            indices[indexCount+3] = 2 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 0 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }

        // BOTTOM
        U32 bottomIndex = xyzToIndexOobCheck(blockPos->x, blockPos->y - 1, blockPos->z);
        if (
            (bottomIndex >= BLOCK_COUNT && blocksIsTransparent(worldGetBlock(position->x + blockPos->x, position->y + blockPos->y - 1, position->z + blockPos->z))) || 
            (bottomIndex < BLOCK_COUNT && blocksIsTransparent(blocks[bottomIndex]))
        ) {
            vertices[indexIndex  ] = chunkPackVertexData(blockPos->x    , blockPos->y    , blockPos->z    , 0, 1, 5, blocksTextureFace(blockType, 5));
            vertices[indexIndex+1] = chunkPackVertexData(blockPos->x + 1, blockPos->y    , blockPos->z    , 1, 1, 5, blocksTextureFace(blockType, 5));
            vertices[indexIndex+2] = chunkPackVertexData(blockPos->x + 1, blockPos->y    , blockPos->z + 1, 1, 0, 5, blocksTextureFace(blockType, 5));
            vertices[indexIndex+3] = chunkPackVertexData(blockPos->x    , blockPos->y    , blockPos->z + 1, 0, 0, 5, blocksTextureFace(blockType, 5));
            
            indices[indexCount  ] = 1 + indexIndex;
            indices[indexCount+1] = 2 + indexIndex;
            indices[indexCount+2] = 0 + indexIndex;
            indices[indexCount+3] = 2 + indexIndex;
            indices[indexCount+4] = 3 + indexIndex;
            indices[indexCount+5] = 0 + indexIndex;
            
            indexIndex += 4;
            indexCount += 6;
        }
    }

    vectoriClean(blockPos);

    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->vertexCount = indexIndex;
    mesh->indexCount = indexCount;
    return mesh;
}

CHUNK* chunkCreate(VECTORI* position, U8* blocks) {
    CHUNK* chunk = malloc(sizeof(CHUNK));

    glGenVertexArrays(1, &chunk->vaoID);
    glGenBuffers(1, &chunk->vboID);
    glGenBuffers(1, &chunk->eboID);

    chunk->indexCount = 0;
    chunk->position = position;
    chunk->blocks = blocks;

    return chunk;
}

void chunkRender(CHUNK* chunk) {
    if (chunk == NULL) return;
    if (!chunk->indexCount) return;
    
    glBindVertexArray(chunk->vaoID);
    glDrawElements(GL_TRIANGLES, chunk->indexCount, GL_UNSIGNED_INT, 0);
}

void chunkDrestroy(CHUNK* chunk) {
    if (chunk == NULL) return;
    
    glDeleteVertexArrays(1, &chunk->vaoID);
    glDeleteBuffers(1, &chunk->vboID);
    glDeleteBuffers(1, &chunk->eboID);
    vectoriClean(chunk->position);
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
