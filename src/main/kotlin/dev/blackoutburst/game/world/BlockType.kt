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
    SNOW(9, false, Array(6) { Textures.SNOW.ordinal });

    companion object {
        private val values = values()

        fun getByID(id: Byte): BlockType {
            if (id < 0 || id > 9) return ERROR
            return values[id.toInt() + 1]
        }
    }
}