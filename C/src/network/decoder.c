#include <stdlib.h>
#include <string.h>
#include "utils/types.h"
#include "utils/ioUtils.h"
#include "utils/math.h"
#include "utils/vectori.h"
#include "network/packet.h"
#include "world/chunk.h"
#include "world/world.h"
#include "world/worldGenerator.h"

void decodePacketIdentification(U8* buffer) {
    //println("Identification");
}

void decodePacketAddEntity(U8* buffer) {
    //println("Add Entity");
}

void decodePacketRemoveEntity(U8* buffer) {
    //println("Remove Entity");
}

void decodePacketUpdateEntity(U8* buffer) {
    //println("Update Entity");
}

void decodePacketSendChunk(U8* buffer) {
    U8** bufferptr = &buffer;
    VECTORI* position = vectoriInit();
    vectoriSet(position, getI32(bufferptr), getI32(bufferptr), getI32(bufferptr), 0);
    U8* blocks = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    for (I32 i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) blocks[i] = getU8(bufferptr);

    VECTORI* pa = vectoriInit();
    vectoriSet(pa, position->x, position->y, position->z, 0);
    U8* ba = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    memcpy(ba, blocks, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);

    CHUNK* chunk = chunkCreate(pa, ba);
    worldAddChunk(chunk);

    I32 chunkPoses[6][3] = {
        {position->x + CHUNK_SIZE, position->y, position->z},
        {position->x - CHUNK_SIZE, position->y, position->z},
        {position->x, position->y + CHUNK_SIZE, position->z},
        {position->x, position->y - CHUNK_SIZE, position->z},
        {position->x, position->y, position->z + CHUNK_SIZE},
        {position->x, position->y, position->z - CHUNK_SIZE},
    };

    for (U8 i = 0; i < 6; i++) {
        CHUNK* tmp = worldGetChunk(chunkPoses[i][0], chunkPoses[i][1], chunkPoses[i][2]);
        if (tmp == NULL) continue;
        VECTORI* p = vectoriInit();
        vectoriSet(p, tmp->position->x, tmp->position->y, tmp->position->z, 0);
        U8* b = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
        memcpy(b, tmp->blocks, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
        meshQueuePush(p, b);
    }

    meshQueuePush(position, blocks);
}

void decodePacketSendMonotypeChunk(U8* buffer) {
    U8** bufferptr = &buffer;
    VECTORI* position = vectoriInit();
    vectoriSet(position, getI32(bufferptr), getI32(bufferptr), getI32(bufferptr), 0);
    U8* blocks = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    U8 blockType = getU8(bufferptr);
    for (I32 i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) blocks[i] = blockType;

    VECTORI* pa = vectoriInit();
    vectoriSet(pa, position->x, position->y, position->z, 0);
    U8* ba = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    memcpy(ba, blocks, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);

    CHUNK* chunk = chunkCreate(pa, ba);
    worldAddChunk(chunk);

    I32 chunkPoses[6][3] = {
        {position->x + CHUNK_SIZE, position->y, position->z},
        {position->x - CHUNK_SIZE, position->y, position->z},
        {position->x, position->y + CHUNK_SIZE, position->z},
        {position->x, position->y - CHUNK_SIZE, position->z},
        {position->x, position->y, position->z + CHUNK_SIZE},
        {position->x, position->y, position->z - CHUNK_SIZE},
    };

    for (U8 i = 0; i < 6; i++) {
        CHUNK* tmp = worldGetChunk(chunkPoses[i][0], chunkPoses[i][1], chunkPoses[i][2]);
        if (tmp == NULL) continue;
        VECTORI* p = vectoriInit();
        vectoriSet(p, tmp->position->x, tmp->position->y, tmp->position->z, 0);
        U8* b = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
        memcpy(b, tmp->blocks, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
        meshQueuePush(p, b);
    }

    meshQueuePush(position, blocks);
}

void decodePacketChat(U8* buffer) {
    //println("Chat");
}

void decodePacketUpdateEntityMetadata(U8* buffer) {
    //println("Update Entity Metadata");
}
