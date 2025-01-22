
#include "graphics/shader.h"
#include "utils/math.h"
#include "utils/types.h"
#include "utils/mutex.h"
#include "world/chunk.h"
#include "world/blocks.h"
#include "world/world.h"

static HASH* chunks = NULL;

#if defined(_WIN32) || defined(_WIN64)
    static CRITICAL_SECTION chunkHashMutex;
#else
    static pthread_mutex_t chunkHashMutex;
#endif

CHUNK* worldGetChunk(I32 x, I32 y, I32 z) {
    mutexLock(&chunkHashMutex);

    if (chunks == NULL) {
        mutexUnlock(&chunkHashMutex);
        return NULL;
    }

    U32 index = chunkHash(x, y, z) % CHUNK_COUNT;
    U32 start = index;

    while (1) {
        if (!chunks[index].used) {
            mutexUnlock(&chunkHashMutex);
            return NULL;
        }
        
        if (chunks[index].x == x && chunks[index].y == y && chunks[index].z == z) {
            mutexUnlock(&chunkHashMutex);
            return chunks[index].chunk;
        }

        index = (index + 1) % CHUNK_COUNT;
        
        if (index == start) {
            mutexUnlock(&chunkHashMutex);
            return NULL;
        }
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
    mutexLock(&chunkHashMutex);
    
    if (chunks == NULL && chunk != NULL) {
        mutexUnlock(&chunkHashMutex);
        return;
    }

    U32 index = chunkHash(chunk->position[VX], chunk->position[VY], chunk->position[VZ]) % CHUNK_COUNT;
    U32 start = index;

    while (1) {
        if (!chunks[index].used) {
            chunks[index].x = chunk->position[VX];
            chunks[index].y = chunk->position[VY];
            chunks[index].z = chunk->position[VZ];
            chunks[index].chunk = chunk;
            chunks[index].used = 1;
            
            mutexUnlock(&chunkHashMutex);
            return;
        } else if (chunks[index].used &&
                   chunks[index].x == chunk->position[VX] &&
                   chunks[index].y == chunk->position[VY] &&
                   chunks[index].z == chunk->position[VZ])
        {
            worldRemoveChunk(chunk->position[VX], chunk->position[VY], chunk->position[VZ]);
            chunks[index].x = chunk->position[VX];
            chunks[index].y = chunk->position[VY];
            chunks[index].z = chunk->position[VZ];
            chunks[index].chunk = chunk;
            chunks[index].used = 1;
            mutexUnlock(&chunkHashMutex);
            return;  
        }
        
        index = (index + 1) % CHUNK_COUNT;
        
        if (index == start) {
            mutexUnlock(&chunkHashMutex);
            return;
        }
    }
}

void worldRemoveChunk(I32 x, I32 y, I32 z) {
    mutexLock(&chunkHashMutex);
    
    U32 index = chunkHash(x, y, z) % CHUNK_COUNT;
    U32 start = index;

    while (1) {
        if (chunks[index].used &&
            chunks[index].x == x &&
            chunks[index].y == y &&
            chunks[index].z == z)
        {
            chunkDrestroy(chunks[index].chunk);
        
            chunks[index].x = 0;
            chunks[index].y = 0;
            chunks[index].z = 0;
            chunks[index].chunk = NULL;
            chunks[index].used = 0;

            mutexUnlock(&chunkHashMutex);
            return;
        }
        
        index = (index + 1) % CHUNK_COUNT;
        
        if (index == start) {
            mutexUnlock(&chunkHashMutex);
            return;
        }
    }
}

void worldRemoveChunkOutOfRenderDistance(U8 renderDistance, I32 x, I32 y, I32 z) {
    mutexLock(&chunkHashMutex);

    if (chunks == NULL) {
        mutexUnlock(&chunkHashMutex);
        return;
    }

    I32 px = x / CHUNK_SIZE;
    I32 py = y / CHUNK_SIZE;
    I32 pz = z / CHUNK_SIZE;

    for (U16 i = 0; i < CHUNK_COUNT; i++) {
        if (!chunks[i].used) continue;

        I32 cx = chunks[i].x / CHUNK_SIZE;
        I32 cy = chunks[i].y / CHUNK_SIZE;
        I32 cz = chunks[i].z / CHUNK_SIZE;

        if (abs(cx - px) > renderDistance ||
            abs(cy - py) > renderDistance ||
            abs(cz - pz) > renderDistance)
        {
            worldRemoveChunk(chunks[i].x, chunks[i].y, chunks[i].z);
        }
    }

    mutexUnlock(&chunkHashMutex);
}

void worldRender(I32 shaderProgram) {
    if (chunks == NULL) return;
    
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        if (chunks[i].chunk == NULL) continue;
        setUniform3f(shaderProgram, "chunkPos", chunks[i].chunk->position[VX], chunks[i].chunk->position[VY], chunks[i].chunk->position[VZ]);
        chunkRender(chunks[i].chunk);
    }
}

void worldClean() {
    if (chunks == NULL) return;
    
    for (U32 i = 0; i < CHUNK_COUNT; i++) chunkDrestroy(chunks[i].chunk);
    free(chunks);

    mutexDestroy(&chunkHashMutex);
}

void worldInit() {
    if (chunks != NULL) return;
    
    chunks = malloc(sizeof(HASH) * CHUNK_COUNT);
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        chunks[i].x = 0;
        chunks[i].y = 0;
        chunks[i].z = 0;
        chunks[i].chunk = NULL;
        chunks[i].used = 0;
    }

    mutexCreate(&chunkHashMutex);
}
