#pragma once
#include "utils/types.h"

typedef struct C00PacketIdentification C00IDENTIFICATION
typedef struct C01PacketAddEntity C01ADD_ENTITY
typedef struct C02RemoveEntity C02REMOVE_ENTITY
typedef struct C03UpdateEntity C03UPDATE_ENTITY
typedef struct C04SendChunk C04SEND_CHUNK
typedef struct C05SendMonotypeChunk C05SEND_MONOTYPE_CHUNK
typedef struct C06Chat C06CHAT
typedef struct C07UpdateEntityMetadata C07UPDATE_ENTITY_METADATA

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
    I8[64] name;
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
    I8[4096] blocks;
};

struct C05SendMonotypeChunk {
    I32 x;
    I32 y;
    I32 z;
    I8 type;
};

struct C06Chat {
    I8[4096] message;
};

struct C07UpdateEntityMetadata {
    U32 entityId;
    I8[64] name;
};
