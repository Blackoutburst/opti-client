#include <stdlib.h>
#include "utils/math.h"
#include "network/encoder.h"
#include "network/packet.h"

U8* encodePacketUpdateEntity(S00UPDATE_ENTITY* packet) {
    U8* buffer = malloc(sizeof(S00UPDATE_ENTITY));
    for (U16 i = 0; i < sizeof(S00UPDATE_ENTITY); i++) buffer[i] = 0;

    U16 index = 0;

    putU8(&index, packet->id, buffer);
    putF32(&index, packet->x, buffer);
    putF32(&index, packet->y, buffer);
    putF32(&index, packet->z, buffer);
    putF32(&index, rad(packet->yaw), buffer);
    putF32(&index, rad(packet->pitch), buffer);

    return buffer;
}

U8* encodePacketUpdateBlock(S01UPDATE_BLOCK* packet) {
    U8* buffer = malloc(sizeof(S01UPDATE_BLOCK));
    for (U16 i = 0; i < sizeof(S01UPDATE_BLOCK); i++) buffer[i] = 0;

    U16 index = 0;

    putU8(&index, packet->id, buffer);
    putU8(&index, packet->type, buffer);
    putI32(&index, packet->x, buffer);
    putI32(&index, packet->y, buffer);
    putI32(&index, packet->z, buffer);

    return buffer;
}

U8* encodePacketBlockBulkEdit(S02BLOCK_BULK_EDIT* packet) {
    U8* buffer = malloc(sizeof(S02BLOCK_BULK_EDIT));
    for (U16 i = 0; i < sizeof(S02BLOCK_BULK_EDIT); i++) buffer[i] = 0;

    //U16 index = 0;

    //TODO

    return buffer;
}

U8* encodePacketChat(S03CHAT* packet) {
    U8* buffer = malloc(sizeof(S03CHAT));
    for (U16 i = 0; i < sizeof(S03CHAT); i++) buffer[i] = 0;

    //U16 index = 0;
    
    //TODO

    return buffer;
}

U8* encodePacketClientMetadata(S04CLIENT_METADATA* packet) {
    U8* buffer = malloc(sizeof(S04CLIENT_METADATA));
    for (U16 i = 0; i < sizeof(S04CLIENT_METADATA); i++) buffer[i] = 0;

    U16 index = 0;
    
    putU8(&index, packet->id, buffer);
    putU8(&index, packet->renderDistance, buffer);
    for (U8 i = 0; packet->name[i]; i++) putU8(&index, packet->name[i], buffer);

    return buffer;
}

U8* encodeString(const U8* str, U16 size) {
    U8* string = malloc(size);
    for (U8 i = 0; i < size; i++) string[i] = 0;
    
    for (U16 i = 0; i < size && str[i]; i++) string[i] = str[i];

    return string;
}

