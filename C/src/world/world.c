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
        
        if (chunks[index].x == x && chunks[index].y == y && chunks[index].z == z) {
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

    U32 index = chunkHash(chunk->position[VX], chunk->position[VY], chunk->position[VZ]) % CHUNK_COUNT;
    U32 start = index;

    while (1) {
        if (!chunks[index].used) {
            chunks[index].x = chunk->position[VX];
            chunks[index].y = chunk->position[VY];
            chunks[index].z = chunk->position[VZ];
            chunks[index].chunk = chunk;
            chunks[index].used = 1;
            
            mutexUnlock(&mutex);
            return;
        } else if (chunks[index].used &&
                   chunks[index].x == chunk->position[VX] &&
                   chunks[index].y == chunk->position[VY] &&
                   chunks[index].z == chunk->position[VZ])
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

void worldRemoveChunkOutOfRenderDistance(U8 renderDistance, I32 x, I32 y, I32 z) {
    mutexLock(&mutex);

    if (chunks == NULL) {
        mutexUnlock(&mutex);
        return;
    }

    I32 px = (x < 0 ? (x + 1) / CHUNK_SIZE - 1 : x / CHUNK_SIZE) * CHUNK_SIZE;
    I32 py = (y < 0 ? (y + 1) / CHUNK_SIZE - 1 : y / CHUNK_SIZE) * CHUNK_SIZE;
    I32 pz = (z < 0 ? (z + 1) / CHUNK_SIZE - 1 : z / CHUNK_SIZE) * CHUNK_SIZE;

    for (U16 i = 0; i < CHUNK_COUNT; i++) {
        if (!chunks[i].used) continue;

        if (abs(chunks[i].x - px) > (renderDistance * CHUNK_SIZE) ||
            abs(chunks[i].y - py) > (renderDistance * CHUNK_SIZE) ||
            abs(chunks[i].z - pz) > (renderDistance * CHUNK_SIZE))
        {
            _worldRemoveChunk(chunks[i].x, chunks[i].y, chunks[i].z);
        }
    }

    mutexUnlock(&mutex);
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

    mutexDestroy(&mutex);
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

    mutexCreate(&mutex);
}
