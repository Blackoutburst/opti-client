#include <stdlib.h>
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
    meshQueuePush(position, blocks, 0);
}

void decodePacketSendMonotypeChunk(U8* buffer) {
    U8** bufferptr = &buffer;
    VECTORI* position = vectoriInit();
    vectoriSet(position, getI32(bufferptr), getI32(bufferptr), getI32(bufferptr), 0);
    U8* blocks = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    U8 blockType = getU8(bufferptr);
    for (I32 i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) blocks[i] = blockType;
    meshQueuePush(position, blocks, 0);
}

void decodePacketChat(U8* buffer) {
    //println("Chat");
}

void decodePacketUpdateEntityMetadata(U8* buffer) {
    //println("Update Entity Metadata");
}
