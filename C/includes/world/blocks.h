#pragma once

#include "utils/types.h"

enum BlockType {
    UNKNOWN = -1,
    AIR = 0,
    GRASS = 1,
    DIRT = 2,
    STONE = 3,
    OAK_LOG = 4,
    OAK_LEAVES = 5,
    GLASS = 6,
    WATER = 7,
    SAND = 8,
    SNOW = 9,
    OAK_PLANKS = 10,
    STONE_BRICKS = 11,
    NETHERRACK = 12,
    GOLD_BLOCK = 13,
    PACKED_ICE = 14,
    LAVA = 15,
    BARREL = 16,
    BOOKSHELF = 17
};

enum BlockTextures {
    TEXTURE_ERROR,
    TEXTURE_GRASS_TOP,
    TEXTURE_GRASS_SIDE,
    TEXTURE_DIRT,
    TEXTURE_STONE,
    TEXTURE_LOG_OAK_TOP,
    TEXTURE_LOG_OAK,
    TEXTURE_LEAVES_OAK,
    TEXTURE_GLASS,
    TEXTURE_WATER,
    TEXTURE_SAND,
    TEXTURE_SNOW,
    TEXTURE_OAK_PLANKS,
    TEXTURE_STONE_BRICKS,
    TEXTURE_NETHERRACK,
    TEXTURE_GOLD_BLOCK,
    TEXTURE_PACKED_ICE,
    TEXTURE_LAVA,
    TEXTURE_BARREL_TOP,
    TEXTURE_BARREL_SIDE,
    TEXTURE_BARREL_BOTTOM,
    TEXTURE_BOOKSHELF
};

U8 blocksIsTransparent(U8 blockType);
U8 blocksTextureFace(U8 blockType, U8 face);
