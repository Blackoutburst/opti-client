#pragma once

#include "utils/types.h"
#include "network/packet.h"

U8* encodePacketUpdateEntity(S00UPDATE_ENTITY* packet);
U8* encodePacketUpdateBlock(S01UPDATE_BLOCK* packet);
U8* encodePacketBlockBulkEdit(S02BLOCK_BULK_EDIT* packet);
U8* encodePacketChat(S03CHAT* packet);
U8* encodePacketClientMetadata(S04CLIENT_METADATA* packet);
U8* encodeString(const U8* str, U16 size);
