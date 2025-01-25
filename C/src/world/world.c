#include "graphics/shader.h"
#include "utils/math.h"
#include "utils/types.h"
#include "utils/mutex.h"
#include "world/chunk.h"
#include "world/blocks.h"
#include "world/world.h"

static HASH* chunks = NULL;

#if defined(_WIN32) || defined(_WIN64)
    static CRITICAL_SECTION mutex;
#else
    static pthread_mutex_t mutex;
#endif


CHUNK* _worldGetChunk(I32 x, I32 y, I32 z) {
    if (chunks == NULL) {
        return NULL;
    }

    U32 index = chunkHash(x, y, z) % CHUNK_COUNT;
    U32 start = index;

    while (1) {
        if (!chunks[index].used) {
            return NULL;
        }
        
        if (chunks[index].position->x == x && chunks[index].position->y == y && chunks[index].position->z == z) {
            return chunks[index].chunk;
        }

        index = (index + 1) % CHUNK_COUNT;
        
        if (index == start) {
            return NULL;
        }
    }
}

CHUNK* worldGetChunk(I32 x, I32 y, I32 z) {
    mutexLock(&mutex);

    CHUNK* chunk = _worldGetChunk(x, y, z);
    
    mutexUnlock(&mutex);

    return chunk;
}

U8 worldGetBlock(I32 x, I32 y, I32 z) {
    mutexLock(&mutex);
    I32 cx = (x < 0 ? (x + 1) / CHUNK_SIZE - 1 : x / CHUNK_SIZE) * CHUNK_SIZE;
    I32 cy = (y < 0 ? (y + 1) / CHUNK_SIZE - 1 : y / CHUNK_SIZE) * CHUNK_SIZE;
    I32 cz = (z < 0 ? (z + 1) / CHUNK_SIZE - 1 : z / CHUNK_SIZE) * CHUNK_SIZE;
    
    CHUNK* chunk = _worldGetChunk(cx, cy, cz);
    if (chunk == NULL)  {
        mutexUnlock(&mutex);
        return 0;
    }
    
    I32 bx = ((x - cx) % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;
    I32 by = ((y - cy) % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;
    I32 bz = ((z - cz) % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;
    
    U32 index = xyzToIndex(bx, by, bz);
    U8 block = chunk->blocks[index];
    
    mutexUnlock(&mutex);
    
    return block;
}

void worldAddChunk(CHUNK* chunk) {
    mutexLock(&mutex);
    
    if (chunks == NULL && chunk != NULL) {
        mutexUnlock(&mutex);
        return;
    }

    U32 index = chunkHash(chunk->position->x, chunk->position->y, chunk->position->z) % CHUNK_COUNT;
    U32 start = index;

    while (1) {
        if (!chunks[index].used) {
            vectoriSet(chunks[index].position, chunk->position->x, chunk->position->y, chunk->position->z, 0);
            chunks[index].chunk = chunk;
            chunks[index].used = 1;
            
            mutexUnlock(&mutex);
            return;
        } else if (chunks[index].used && vectoriEquals(chunks[index].position, chunk->position))
        {
            chunkDrestroy(chunks[index].chunk);
            chunks[index].chunk = chunk;
            mutexUnlock(&mutex);
            return;  
        }
        
        index = (index + 1) % CHUNK_COUNT;
        
        if (index == start) {
            mutexUnlock(&mutex);
            return;
        }
    }
}

void _worldRemoveChunk(I32 x, I32 y, I32 z) {
    U32 index = chunkHash(x, y, z) % CHUNK_COUNT;
    U32 start = index;

    while (1) {
        if (chunks[index].used &&
            chunks[index].position->x == x &&
            chunks[index].position->y == y &&
            chunks[index].position->z == z)
        {
            chunks[index].used = 0;
            chunkDrestroy(chunks[index].chunk);
            vectoriSet(chunks[index].position, 0, 0, 0, 0);
            chunks[index].chunk = NULL;

            return;
        }
        
        index = (index + 1) % CHUNK_COUNT;
        
        if (index == start) {
            return;
        }
    }
}

void worldRemoveChunk(I32 x, I32 y, I32 z) {
    mutexLock(&mutex);

    _worldRemoveChunk(x, y, z);
    
    mutexUnlock(&mutex);
}

void worldRemoveChunkOutOfRenderDistance(I32 renderDistance, I32 x, I32 y, I32 z) {
    mutexLock(&mutex);

    if (chunks == NULL) {
        mutexUnlock(&mutex);
        return;
    }

    I32 px = (x < 0 ? (x + 1) / CHUNK_SIZE - 1 : x / CHUNK_SIZE) * CHUNK_SIZE;
    I32 py = (y < 0 ? (y + 1) / CHUNK_SIZE - 1 : y / CHUNK_SIZE) * CHUNK_SIZE;
    I32 pz = (z < 0 ? (z + 1) / CHUNK_SIZE - 1 : z / CHUNK_SIZE) * CHUNK_SIZE;

    for (U16 i = 0; i < CHUNK_COUNT; i++) {
        if (!chunks[i].used || chunks[i].chunk == NULL) continue;

        if (abs(chunks[i].position->x - px) > (renderDistance * CHUNK_SIZE) ||
            abs(chunks[i].position->y - py) > (renderDistance * CHUNK_SIZE) ||
            abs(chunks[i].position->z - pz) > (renderDistance * CHUNK_SIZE))
        {
            _worldRemoveChunk(chunks[i].position->x, chunks[i].position->y, chunks[i].position->z);
        }
    }

    mutexUnlock(&mutex);
}

void worldRender(I32 shaderProgram) {
    if (chunks == NULL) return;
    
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        if (!chunks[i].used || chunks[i].chunk == NULL) continue;
        setUniform3f(shaderProgram, "chunkPos", chunks[i].chunk->position->x, chunks[i].chunk->position->y, chunks[i].chunk->position->z);
        chunkRender(chunks[i].chunk);
    }
}

void worldClean() {
    if (chunks == NULL) return;
    
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        vectoriClean(chunks[i].position);
        chunkDrestroy(chunks[i].chunk);
    }
    
    free(chunks);

    mutexDestroy(&mutex);
}

void worldInit() {
    if (chunks != NULL) return;

    chunks = malloc(sizeof(HASH) * CHUNK_COUNT);
    for (U32 i = 0; i < CHUNK_COUNT; i++) {
        chunks[i].position = vectoriInit();
        chunks[i].chunk = NULL;
        chunks[i].used = 0;
    }

    mutexCreate(&mutex);
}
