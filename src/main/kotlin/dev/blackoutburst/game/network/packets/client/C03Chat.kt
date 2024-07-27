package dev.blackoutburst.game.network.packets.client

import dev.blackoutburst.game.network.packets.PacketPlayOut
import dev.blackoutburst.game.world.Block
import java.nio.ByteBuffer
import java.nio.ByteOrder

private const val ID: Byte = 0x03

class C03Chat(
    private val message: String
) : PacketPlayOut() {

    init {
        buffer = ByteBuffer.allocate(4097).clear()

        buffer?.apply {
            order(ByteOrder.BIG_ENDIAN)
            put(ID)
            message.encodeToByteArray().forEach { put(it) }
        }
    }
}