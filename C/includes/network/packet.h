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

typedef struct BlockBulkEdit BLOCK_BULK_EDIT;

typedef struct S00UpdateEntity S00UPDATE_ENTITY;
typedef struct S01UpdateBlock S01UPDATE_BLOCK;
typedef struct S02BlockBulkEdit S02BLOCK_BULK_EDIT;
typedef struct S03Chat S03CHAT;
typedef struct S04ClientMetadata S04CLIENT_METADATA;

#pragma pack(push, 1)
struct C00PacketIdentification {
    U32 entityId;
};

struct C01PacketAddEntity {
    U32 entityId;
    F32 x;
    F32 y;
    F32 z;
    F32 yaw;
    F32 pitch;
    U8 name[64];
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
    U8 blocks[4096];
};

struct C05SendMonotypeChunk {
    I32 x;
    I32 y;
    I32 z;
    U8 type;
};

struct C06Chat {
    U8 message[4096];
};

struct C07UpdateEntityMetadata {
    U32 entityId;
    U8 name[64];
};

struct S00UpdateEntity {
    U8 id;
    F32 x;
    F32 y;
    F32 z;
    F32 yaw;
    F32 pitch;
};

struct S01UpdateBlock {
    U8 id;
    U8 type;
    I32 x;
    I32 y;
    I32 z;
};

struct BlockBulkEdit {
    U8 type;
    I32 x;
    I32 y;
    I32 z;
};

struct S02BlockBulkEdit {
    U8 id;
    U32 blockCount;
    BLOCK_BULK_EDIT* blocks;
};

struct S03Chat {
    U8 id;
    U8 message[4096];
};

struct S04ClientMetadata {
    U8 id;
    U8 renderDistance;
    U8 name[64];
};

#pragma pack(pop)


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

enum ServerPackets {
    SERVER_PACKET_UPDATE_ENTITY,
    SERVER_PACKET_UPDATE_BLOCK,
    SERVER_PACKET_BLOCK_BULK_EDIT,
    SERVER_PACKET_CHAT,
    SERVER_PACKET_CLIENT_METADATA,
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

void putI8(U16* index, I8 value, U8* buffer);
void putU8(U16* index, U8 value, U8* buffer);
void putI16(U16* index, I16 value, U8* buffer);
void putU16(U16* index, U16 value, U8* buffer);
void putI32(U16* index, I32 value, U8* buffer);
void putU32(U16* index, U32 value, U8* buffer);
void putI64(U16* index, I64 value, U8* buffer);
void putU64(U16* index, U64 value, U8* buffer);
void putF32(U16* index, F32 value, U8* buffer);
void putF64(U16* index, F64 value, U8* buffer);

void* getClientPacketfunction(I8 packetID);
U16 getClientPacketSize(I8 packetID);
U16 getServerPacketSize(I8 packetID);

void packetSendUpdateEntity(I32 x, I32 y, I32 z, F32 yaw, F32 pitch);
void packetSendUpdateBlock(U8 type, I32 x, I32 y, I32 z);
void packetSendBlockBulkEdit(U32 blockCount, BLOCK_BULK_EDIT* blocks);
void packetSendChat(const U8* message);
void packetSendClientMetadata(U8 renderDistance, const U8* name);
