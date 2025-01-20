#include <stdlib.h>
#include "world/chunk.h"
#include "world/world.h"
#include "utils/ioUtils.h"
#include "graphics/shader.h"
#include "utils/math.h"

static HASH* chunks = NULL;

void worldReMeshAdjacentChunks(CHUNK* chunk) {
    // TOP
    CHUNK* top = worldGetChunk(chunk->position[VX], chunk->position[VY] + CHUNK_SIZE, chunk->position[VZ]);
    if (top != NULL) {
        I32* m = generateChunkMesh(top);
        generateChunkVAO(top, m);
    }

    // FRONT
    CHUNK* front = worldGetChunk(chunk->position[VX], chunk->position[VY], chunk->position[VZ] - CHUNK_SIZE);
    if (front != NULL) {
        I32* m = generateChunkMesh(front);
        generateChunkVAO(front, m);
    }

    // BACK
    CHUNK* back = worldGetChunk(chunk->position[VX], chunk->position[VY], chunk->position[VZ] + CHUNK_SIZE);
    if (back != NULL) {
        I32* m = generateChunkMesh(back);
        generateChunkVAO(back, m);
    }

    // LEFT
    CHUNK* left = worldGetChunk(chunk->position[VX] - CHUNK_SIZE, chunk->position[VY], chunk->position[VZ]);
    if (left != NULL) {
        I32* m = generateChunkMesh(left);
        generateChunkVAO(left, m);
    }

    // RIGHT
    CHUNK* right = worldGetChunk(chunk->position[VX] + CHUNK_SIZE, chunk->position[VY], chunk->position[VZ]);
    if (right != NULL) {
        I32* m = generateChunkMesh(right);
        generateChunkVAO(right, m);
    }

    // BOTTOM
    CHUNK* bottom = worldGetChunk(chunk->position[VX], chunk->position[VY] - CHUNK_SIZE, chunk->position[VZ]);
    if (bottom != NULL) {
        I32* m = generateChunkMesh(bottom);
        generateChunkVAO(bottom, m);
    }
}

CHUNK* worldGetChunk(I32 x, I32 y, I32 z) {
    if (chunks == NULL) return NULL;

    U32 index = chunkHash(x, y, z) % CHUNK_COUNT;
    U32 start = index;

    while (1) {
        if (!chunks[index].used) return NULL;
        if (chunks[index].x == x && chunks[index].y == y && chunks[index].z == z)
            return chunks[index].chunk;

        index = (index + 1) % CHUNK_COUNT;
        if (index == start) return NULL;
    }
}

U8 worldGetBlock(I32 x, I32 y, I32 z) {
    I32 cx = (x < 0 ? (x + 1) / CHUNK_SIZE - 1 : x / CHUNK_SIZE) * CHUNK_SIZE;
    I32 cy = (y < 0 ? (y + 1) / CHUNK_SIZE - 1 : y / CHUNK_SIZE) * CHUNK_SIZE;
    I32 cz = (z < 0 ? (z + 1) / CHUNK_SIZE - 1 : z / CHUNK_SIZE) * CHUNK_SIZE;
    CHUNK* chunk = worldGetChunk(cx, cy, cz);
    if (chunk == NULL) return 0;
    
    I32 bx = ((x - cx) % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;
    I32 by = ((y - cy) % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;
    I32 bz = ((z - cz) % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;
    U32 index = xyzToIndex(bx, by, bz);
    
    return chunk->blocks[index];
}

void worldAddChunk(CHUNK* chunk) {
    if (chunks == NULL && chunk != NULL) return;

    U32 index = chunkHash(chunk->position[VX], chunk->position[VY], chunk->position[VZ]) % CHUNK_COUNT;
    U32 start = index;

    while (1) {
        if (!chunks[index].used) {
            chunks[index].x = chunk->position[VX];
            chunks[index].y = chunk->position[VY];
            chunks[index].z = chunk->position[VZ];
            chunks[index].chunk = chunk;
            chunks[index].used = 1;
            return;
        }
        index = (index + 1) % CHUNK_COUNT;
        if (index == start) return;
    }
}

void worldRemoveChunk(I32 x, I32 y, I32 z) {
    if (chunks == NULL) return;
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        if (chunks[i].chunk == NULL) continue;
        if (chunks[i].chunk->position[VX] == x &&
            chunks[i].chunk->position[VY] == y &&
            chunks[i].chunk->position[VZ] == z
        ) {
            destroyChunk(chunks[i].chunk);
            chunks[i].chunk = NULL;
            chunks[i].used = 0;
            break;
        }
    }
}

void worldRender(I32 shaderProgram) {
    if (chunks == NULL) return;
    
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        if (chunks[i].chunk == NULL) continue;
        setUniform3f(shaderProgram, "chunkPos", chunks[i].chunk->position[VX], chunks[i].chunk->position[VY], chunks[i].chunk->position[VZ]);
        renderChunk(chunks[i].chunk);
    }
}

void worldClean() {
    if (chunks == NULL) return;
    
    cleanChunkMemoryRegion();
    for (U32 i = 0; i < CHUNK_COUNT; i++) destroyChunk(chunks[i].chunk);
    free(chunks);
}

void worldInit() {
    if (chunks != NULL) return;
    
    initChunkMemoryRegion();
    chunks = malloc(sizeof(HASH) * CHUNK_COUNT);
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        chunks[i].x = 0;
        chunks[i].y = 0;
        chunks[i].z = 0;
        chunks[i].chunk = NULL;
        chunks[i].used = 0;
    }
}
