#include "utils/types.h"
#include "world/blocks.h"

U8 blocksIsTransparent(U8 blockType) {
    switch (blockType) {
        case AIR: case OAK_LEAVES: case GLASS: return 1;
        default: return 0;
    }
}

U8 blocksTextureFace(U8 blockType, U8 face) {
    switch (blockType) {
        case GRASS: {
            switch (face) {
                case 0: return TEXTURE_GRASS_TOP;
                case 1: case 2: case 3: case 4: return TEXTURE_GRASS_SIDE;
                case 5: return TEXTURE_DIRT;
            }
        }
        case DIRT: return TEXTURE_DIRT;
        case STONE: return TEXTURE_STONE;
        case OAK_LOG: {
            switch (face) {
                case 0: case 5: return TEXTURE_LOG_OAK_TOP;
                case 1: case 2: case 3: case 4: return TEXTURE_LOG_OAK;
            }
        }
        case OAK_LEAVES: return TEXTURE_LEAVES_OAK;
        case GLASS: return TEXTURE_GLASS;
        case WATER: return TEXTURE_WATER;
        case SAND: return TEXTURE_SAND;
        case SNOW: return TEXTURE_SNOW;
        default: return TEXTURE_ERROR;
    }
}
