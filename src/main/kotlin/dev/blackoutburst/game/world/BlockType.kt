package dev.blackoutburst.game.world

import dev.blackoutburst.game.utils.Textures

enum class BlockType(val id: Byte, val transparent: Boolean, val textures: Array<Int>) {
    ERROR(-1, false, Array(6) { Textures.ERROR.ordinal }),
    AIR(0, true, emptyArray()),
    GRASS(1, false, arrayOf(Textures.GRASS_TOP.ordinal, Textures.GRASS_SIDE.ordinal, Textures.GRASS_SIDE.ordinal, Textures.GRASS_SIDE.ordinal, Textures.GRASS_SIDE.ordinal, Textures.DIRT.ordinal)),
    DIRT(2, false, Array(6) { Textures.DIRT.ordinal }),
    STONE(3, false, Array(6) { Textures.STONE.ordinal }),
    OAK_LOG(4, false, arrayOf(Textures.OAK_LOG_TOP.ordinal, Textures.OAK_SIDE.ordinal, Textures.OAK_SIDE.ordinal, Textures.OAK_SIDE.ordinal, Textures.OAK_SIDE.ordinal, Textures.OAK_LOG_TOP.ordinal)),
    OAK_LEAVES(5, true, Array(6) { Textures.OAK_LEAVES.ordinal }),
    GLASS(6, true, Array(6) { Textures.GLASS.ordinal }),
    WATER(7, false, Array(6) { Textures.WATER.ordinal }),
    SAND(8, false, Array(6) { Textures.SAND.ordinal }),
    SNOW(9, false, Array(6) { Textures.SNOW.ordinal }),
    OAK_PLANKS(10, false, Array(6) { Textures.OAK_PLANKS.ordinal }),
    STONE_BRICKS(11, false, Array(6) { Textures.STONE_BRICKS.ordinal }),
    NETHERRACK(12, false, Array(6) { Textures.NETHERRACK.ordinal }),
    GOLD_BLOCK(13, false, Array(6) { Textures.GOLD_BLOCK.ordinal }),
    PACKED_ICE(14, false, Array(6) { Textures.PACKED_ICE.ordinal }),
    LAVA(15, false, Array(6) { Textures.LAVA.ordinal }),
    BARREL(16, false, arrayOf(Textures.BARREL_TOP.ordinal, Textures.BARREL_SIDE.ordinal, Textures.BARREL_SIDE.ordinal, Textures.BARREL_SIDE.ordinal, Textures.BARREL_SIDE.ordinal, Textures.BARREL_BOTTOM.ordinal)),
    BOOKSHELF(17, false, arrayOf(Textures.OAK_PLANKS.ordinal, Textures.BOOKSHELF.ordinal, Textures.BOOKSHELF.ordinal, Textures.BOOKSHELF.ordinal, Textures.BOOKSHELF.ordinal, Textures.OAK_PLANKS.ordinal));

    companion object {
        private val values = values()

        fun getByID(id: Byte): BlockType {
            if (id < 0 || id > 17) return ERROR
            return values[id.toInt() + 1]
        }
    }
}