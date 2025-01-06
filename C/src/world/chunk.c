#include <stdio.h>
#include "world/chunk.h"
#include "world/world.h"

void printChunk(CHUNK chunk) {
    printf("vao: %i, vbo: %i, ebo: %i, index count: %u position: %p blocks: %p\n", chunk.vaoID, chunk.vboID, chunk.eboID, chunk.indexCount, chunk.position, chunk.blocks);
}

CHUNK createChunk() {
    int position[3] = {0};
    char blocks[CHUNK_SIZE * CHUNK_SIZE] = {0};
    CHUNK chunk = {-1, -1, -1, 0, position, blocks};
    return chunk;
}
