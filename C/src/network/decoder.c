#include "utils/types.h"
#include "utils/ioUtils.h"
#include "utils/math.h"
#include "network/packet.h"
#include "world/chunk.h"
#include "world/world.h"
#include "world/worldGenerator.h"
#include <stdlib.h>

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
    I32* position = malloc(sizeof(I32) * 3);
    position[VX] = getI32(bufferptr);
    position[VY] = getI32(bufferptr);
    position[VZ] = getI32(bufferptr);
    U8* blocks = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    for (I32 i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) blocks[i] = getU8(bufferptr);
    CHUNK* chunk = createChunk(position, blocks);
    worldAddChunk(chunk);
    wgQueuePush(chunk);
}

void decodePacketSendMonotypeChunk(U8* buffer) {
    U8** bufferptr = &buffer;
    I32* position = malloc(sizeof(I32) * 3);
    position[VX] = getI32(bufferptr);
    position[VY] = getI32(bufferptr);
    position[VZ] = getI32(bufferptr);
    U8* blocks = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    U8 blockType = getU8(bufferptr);
    for (I32 i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) blocks[i] = blockType;
    CHUNK* chunk = createChunk(position, blocks);
    worldAddChunk(chunk);
    wgQueuePush(chunk);
}

void decodePacketChat(U8* buffer) {
    //println("Chat");
}

void decodePacketUpdateEntityMetadata(U8* buffer) {
    //println("Update Entity Metadata");
}
