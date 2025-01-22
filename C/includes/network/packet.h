#pragma once

#include "utils/types.h"

typedef struct C00PacketIdentification C00IDENTIFICATION;
typedef struct C01PacketAddEntity C01ADD_ENTITY;
typedef struct C02RemoveEntity C02REMOVE_ENTITY;
typedef struct C03UpdateEntity C03UPDATE_ENTITY;
typedef struct C04SendChunk C04SEND_CHUNK;
typedef struct C05SendMonotypeChunk C05SEND_MONOTYPE_CHUNK;
typedef struct C06Chat C06CHAT;
typedef struct C07UpdateEntityMetadata C07UPDATE_ENTITY_METADATA;

struct C00PacketIdentification {
    U32 entityId;
};

struct C01PacketAddEntity {
    I32 entityId;
    F32 x;
    F32 y;
    F32 z;
    F32 yaw;
    F32 pitch;
    I8 name[64];
};

struct C02RemoveEntity {
    U32 entityId;
};

struct C03UpdateEntity {
    U32 entityId;
    F32 x;
    F32 y;
    F32 z;
    F32 yaw;
    F32 pitch;
};

struct C04SendChunk {
    I32 x;
    I32 y;
    I32 z;
    I8 blocks[4096];
};

struct C05SendMonotypeChunk {
    I32 x;
    I32 y;
    I32 z;
    I8 type;
};

struct C06Chat {
    I8 message[4096];
};

struct C07UpdateEntityMetadata {
    U32 entityId;
    I8 name[64];
};


enum ClientPackets {
    CLIENT_PACKET_IDENTIFICATION,
    CLIENT_PACKET_ADD_ENTITY,
    CLIENT_PACKET_REMOVE_ENTITY,
    CLIENT_PACKET_UPDATE_ENTITY,
    CLIENT_PACKET_SEND_CHUNK,
    CLIENT_PACKET_SEND_MONOTYPE_CHUNK,
    CLIENT_PACKET_CHAT,
    CLIENT_PACKET_UPDATE_ENTITY_METADATA,
};

void printBufferHex(const I8 *title, const U8 *buf, U32 buf_len);

I8 getI8(U8** buffer);
U8 getU8(U8** buffer);
I16 getI16(U8** buffer);
U16 getU16(U8** buffer);
I32 getI32(U8** buffer);
U32 getU32(U8** buffer);
I64 getI64(U8** buffer);
U64 getU64(U8** buffer);
F32 getF32(U8** buffer);
F64 getF64(U8** buffer);
F128 getF128(U8** buffer);

void* getPacketfunction(I8 packetID);
U16 getPacketSize(I8 packetID);
