#include <stdlib.h>
#include "world/chunk.h"
#include "world/world.h"
#include "utils/ioUtils.h"
#include "graphics/shader.h"

static CHUNK** chunks = NULL;

void worldAddChunk(CHUNK* chunk) {
    if (chunks == NULL) return;
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        if (chunks[i] == NULL) {
            chunks[i] = chunk;
            break;
        }
    }
}

void worldRemoveChunk(I32 x, I32 y, I32 z) {
    if (chunks == NULL) return;
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        if (chunks[i] == NULL) continue;
        if (chunks[i]->position[0] == x &&
            chunks[i]->position[1] == y &&
            chunks[i]->position[2] == z) {
            destroyChunk(chunks[i]);
            break;
        }
    }
}

void worldRender(I32 shaderProgram) {
    if (chunks == NULL) return;
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        if (chunks[i] == NULL) continue;
        setUniform3f(shaderProgram, "chunkPos", chunks[i]->position[0], chunks[i]->position[1], chunks[i]->position[2]);
        renderChunk(chunks[i]);
    }
}

void worldClean() {
    if (chunks == NULL) return;
    cleanChunkMemoryRegion();
    for (U32 i = 0; i < CHUNK_COUNT; i++) destroyChunk(chunks[i]);
    free(chunks);
}

void worldInit() {
    if (chunks != NULL) return;
    initChunkMemoryRegion();
    chunks = malloc(sizeof(CHUNK*) * CHUNK_COUNT);
    for (U32 i = 0; i < CHUNK_COUNT; i++) chunks[i] = NULL;
}
