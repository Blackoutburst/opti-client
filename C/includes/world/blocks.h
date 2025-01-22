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
};

U8 blocksIsTransparent(U8 blockType);
U8 blocksTextureFace(U8 blockType, U8 face);
