#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils/types.h"
#include "network/packet.h"
#include "network/decoder.h"
#include "network/encoder.h"
#include "network/client.h"

void printBufferHex(const I8 *title, const U8 *buf, U32 buf_len) {
    printf("%s [ ", title);
    
    for (U32 i = 0 ; i < buf_len ; ++i) {
        printf("%02X%s", buf[i], ( i + 1 ) % 16 == 0 ? "\r\n" : " " );
    }
    
    printf("]\n");
}

I8 getI8(U8** buffer)  {
    I8 data = (I8)**buffer;
    (*buffer)++;

    return data;
}

U8 getU8(U8** buffer) {
    U8 data = (U8)**buffer;
    (*buffer)++;

    return data;
}

I16 getI16(U8** buffer) {
    I16 data = (I16)(
        ((*buffer)[0] << 8) |
        ((*buffer)[1])
    );
    (*buffer) += 2;

    return data;
}

U16 getU16(U8** buffer) {
    U16 data = (U16)(
        ((*buffer)[0] << 8) |
        ((*buffer)[1])
    );
    (*buffer) += 2;

    return data;
}

I32 getI32(U8** buffer) {
    I32 data = (I32)(
        ((*buffer)[0] << 24) |
        ((*buffer)[1] << 16) |
        ((*buffer)[2] <<  8) |
        ((*buffer)[3]      )
    );
    (*buffer) += 4;

    return data;
}

U32 getU32(U8** buffer) {
    U32 data = (U32)(
        ((*buffer)[0] << 24) |
        ((*buffer)[1] << 16) |
        ((*buffer)[2] <<  8) |
        ((*buffer)[3]      )
    );
    (*buffer) += 4;

    return data;
}

I64 getI64(U8** buffer) {
    I64 data = (I64)(
        ((I64)((*buffer)[0]) << 56) |
        ((I64)((*buffer)[1]) << 48) |
        ((I64)((*buffer)[2]) << 40) |
        ((I64)((*buffer)[3]) << 32) |
        ((I64)((*buffer)[4]) << 24) |
        ((I64)((*buffer)[5]) << 16) |
        ((I64)((*buffer)[6]) <<  8) |
        ((I64)((*buffer)[7])      )
    );
    (*buffer) += 8;

    return data;
}

U64 getU64(U8** buffer) {
    U64 data = (U64)(
        ((U64)((*buffer)[0]) << 56) |
        ((U64)((*buffer)[1]) << 48) |
        ((U64)((*buffer)[2]) << 40) |
        ((U64)((*buffer)[3]) << 32) |
        ((U64)((*buffer)[4]) << 24) |
        ((U64)((*buffer)[5]) << 16) |
        ((U64)((*buffer)[6]) <<  8) |
        ((U64)((*buffer)[7])      )
    );
    (*buffer) += 8;

    return data;
}

F32 getF32(U8** buffer) {
    union {
        I32 i;
        F32 f;
    } u;

    u.i = getU32(buffer);
    
    return u.f;
}

F64 getF64(U8** buffer) {
    union {
        I64 i;
        F64 f;
    } u;

    u.i = getU64(buffer);
    
    return u.f;
}

void putI8(U16* index, I8 value, U8* buffer)  {
    buffer[*index] = value;
    
    *index += sizeof(I8);
}

void putU8(U16* index, U8 value, U8* buffer) {
    buffer[*index] = value;
    
   *index += sizeof(U8); 
}

void putI16(U16* index, I16 value, U8* buffer) {
    buffer[*index  ] = (value >> 8) & 0xFF;
    buffer[*index+1] = (value)      & 0xFF;

    *index += sizeof(I16);
}

void putU16(U16* index, U16 value, U8* buffer) {
    buffer[*index  ] = (value >> 8) & 0xFF;
    buffer[*index+1] = (value)      & 0xFF;
    
    *index += sizeof(U16);
}

void putI32(U16* index, I32 value, U8* buffer) {
    buffer[*index  ] = (value >> 24) & 0xFF;
    buffer[*index+1] = (value >> 16) & 0xFF;
    buffer[*index+2] = (value >> 8 ) & 0xFF;
    buffer[*index+3] = (value)       & 0xFF;
    
    *index += sizeof(I32);
}

void putU32(U16* index, U32 value, U8* buffer) {
    buffer[*index  ] = (value >> 24) & 0xFF;
    buffer[*index+1] = (value >> 16) & 0xFF;
    buffer[*index+2] = (value >> 8 ) & 0xFF;
    buffer[*index+3] = (value)       & 0xFF;

    *index += sizeof(U32);
}

void putI64(U16* index, I64 value, U8* buffer) {
    buffer[*index  ] = (value >> 56) & 0xFF;
    buffer[*index+1] = (value >> 48) & 0xFF;
    buffer[*index+2] = (value >> 40) & 0xFF;
    buffer[*index+3] = (value >> 32) & 0xFF;
    buffer[*index+4] = (value >> 24) & 0xFF;
    buffer[*index+5] = (value >> 16) & 0xFF;
    buffer[*index+6] = (value >> 8 ) & 0xFF;
    buffer[*index+7] = (value)       & 0xFF;

    *index += sizeof(I64);
}

void putU64(U16* index, U64 value, U8* buffer) {
    buffer[*index  ] = (value >> 56) & 0xFF;
    buffer[*index+1] = (value >> 48) & 0xFF;
    buffer[*index+2] = (value >> 40) & 0xFF;
    buffer[*index+3] = (value >> 32) & 0xFF;
    buffer[*index+4] = (value >> 24) & 0xFF;
    buffer[*index+5] = (value >> 16) & 0xFF;
    buffer[*index+6] = (value >> 8 ) & 0xFF;
    buffer[*index+7] = (value)       & 0xFF;
    
    *index += sizeof(U64);
}

void putF32(U16* index, F32 value, U8* buffer) {
    union {
        F32 f;
        U32 u;
    } u;
    
    u.f = value;
    
    buffer[*index  ] = (u.u >> 24) & 0xFF;
    buffer[*index+1] = (u.u >> 16) & 0xFF;
    buffer[*index+2] = (u.u >> 8 ) & 0xFF;
    buffer[*index+3] = (u.u)       & 0xFF;

    *index += sizeof(F32);
}

void putF64(U16* index, F64 value, U8* buffer) {
    union {
        F64 f;
        U64 u;
    } u;
    
    u.f = value;
    
    buffer[*index  ] = (u.u >> 56) & 0xFF;
    buffer[*index+1] = (u.u >> 48) & 0xFF;
    buffer[*index+2] = (u.u >> 40) & 0xFF;
    buffer[*index+3] = (u.u >> 32) & 0xFF;
    buffer[*index+4] = (u.u >> 24) & 0xFF;
    buffer[*index+5] = (u.u >> 16) & 0xFF;
    buffer[*index+6] = (u.u >> 8 ) & 0xFF;
    buffer[*index+7] = (u.u)       & 0xFF;

    *index += sizeof(F64);
}

void* getClientPacketfunction(I8 packetID) {
    switch (packetID) {
        case CLIENT_PACKET_IDENTIFICATION:
            return &decodePacketIdentification;
        case CLIENT_PACKET_ADD_ENTITY:
            return &decodePacketAddEntity;
        case CLIENT_PACKET_REMOVE_ENTITY:
            return &decodePacketRemoveEntity;
        case CLIENT_PACKET_UPDATE_ENTITY:
            return &decodePacketUpdateEntity;
        case CLIENT_PACKET_SEND_CHUNK:
            return &decodePacketSendChunk;
        case CLIENT_PACKET_SEND_MONOTYPE_CHUNK:
            return &decodePacketSendMonotypeChunk;
        case CLIENT_PACKET_CHAT:
            return &decodePacketChat;
        case CLIENT_PACKET_UPDATE_ENTITY_METADATA:
            return &decodePacketUpdateEntityMetadata;
        default:
            return NULL;
    }
}

U16 getClientPacketSize(I8 packetID) {
    switch (packetID) {
        case CLIENT_PACKET_IDENTIFICATION:
            return sizeof(C00IDENTIFICATION);
        case CLIENT_PACKET_ADD_ENTITY:
            return sizeof(C01ADD_ENTITY);
        case CLIENT_PACKET_REMOVE_ENTITY:
            return sizeof(C02REMOVE_ENTITY);
        case CLIENT_PACKET_UPDATE_ENTITY:
            return sizeof(C03UPDATE_ENTITY);
        case CLIENT_PACKET_SEND_CHUNK:
            return sizeof(C04SEND_CHUNK);
        case CLIENT_PACKET_SEND_MONOTYPE_CHUNK:
            return sizeof(C05SEND_MONOTYPE_CHUNK);
        case CLIENT_PACKET_CHAT:
            return sizeof(C06CHAT);
        case CLIENT_PACKET_UPDATE_ENTITY_METADATA:
            return sizeof(C07UPDATE_ENTITY_METADATA);
        default:
            return 0;
    }
}

U16 getServerPacketSize(I8 packetID) {
    switch (packetID) {
        case SERVER_PACKET_UPDATE_ENTITY:
            return sizeof(S00UPDATE_ENTITY);
        case SERVER_PACKET_UPDATE_BLOCK:
            return sizeof(S01UPDATE_BLOCK);
        case SERVER_PACKET_BLOCK_BULK_EDIT:
            return sizeof(S02BLOCK_BULK_EDIT);
        case SERVER_PACKET_CHAT:
            return sizeof(S03CHAT);
        case SERVER_PACKET_CLIENT_METADATA:
            return sizeof(S04CLIENT_METADATA);
        default:
            return 0;
    }
}

void packetSendUpdateEntity(I32 x, I32 y, I32 z, F32 yaw, F32 pitch) {
    S00UPDATE_ENTITY* packet = malloc(sizeof(S00UPDATE_ENTITY));
    packet->id = SERVER_PACKET_UPDATE_ENTITY;
    packet->x = x;
    packet->y = y;
    packet->z = z;
    packet->yaw = yaw;
    packet->pitch = pitch;

    U8* buffer = encodePacketUpdateEntity(packet);

    connectionSend(buffer, sizeof(S00UPDATE_ENTITY));
    
    free(buffer);
    free(packet);
}

void packetSendUpdateBlock(U8 type, I32 x, I32 y, I32 z) {
    S01UPDATE_BLOCK* packet = malloc(sizeof(S01UPDATE_BLOCK));
    packet->id = SERVER_PACKET_UPDATE_BLOCK;
    packet->type = type;
    packet->x = x;
    packet->y = y;
    packet->z = z;

    U8* buffer = encodePacketUpdateBlock(packet);

    connectionSend(buffer, sizeof(S01UPDATE_BLOCK));
    
    free(buffer);
    free(packet);
}

void packetSendBlockBulkEdit(U32 blockCount, BLOCK_BULK_EDIT* blocks) {
    S02BLOCK_BULK_EDIT* packet = malloc(sizeof(S02BLOCK_BULK_EDIT));
    packet->id = SERVER_PACKET_BLOCK_BULK_EDIT;
    packet->blockCount = blockCount;
    packet->blocks = blocks;

    U8* buffer = encodePacketBlockBulkEdit(packet);
    U32 totalSize = sizeof(S02BLOCK_BULK_EDIT) + (sizeof(BLOCK_BULK_EDIT) * blockCount);
    connectionSend(buffer, totalSize);
    
    free(buffer);
    free(packet);
}

void packetSendChat(const U8* message) {
    U8* encodedString = encodeString(message, 4096);

    S03CHAT* packet = malloc(sizeof(S03CHAT));
    packet->id = SERVER_PACKET_CHAT;
    memcpy(packet->message, encodedString, 4096);

    U8* buffer = encodePacketChat(packet);

    connectionSend(buffer, sizeof(S03CHAT));
    
    free(buffer);
    free(encodedString);
    free(packet);
}

void packetSendClientMetadata(U8 renderDistance, const U8* name) {
    U8* encodedString = encodeString(name, 64);
    
    S04CLIENT_METADATA* packet = malloc(sizeof(S04CLIENT_METADATA));
    packet->id = SERVER_PACKET_CLIENT_METADATA;
    packet->renderDistance = renderDistance;
    memcpy(packet->name, encodedString, 64);
    
    U8* buffer = encodePacketClientMetadata(packet);
    
    connectionSend(buffer, sizeof(S04CLIENT_METADATA));
    
    free(buffer);
    free(encodedString);
    free(packet);
}
