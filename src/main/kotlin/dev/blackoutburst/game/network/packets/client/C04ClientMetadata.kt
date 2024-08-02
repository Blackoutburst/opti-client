package dev.blackoutburst.game.network.packets.client

import dev.blackoutburst.game.network.packets.PacketPlayOut
import dev.blackoutburst.game.utils.fit
import java.nio.ByteBuffer
import java.nio.ByteOrder

private const val ID: Byte = 0x04

class C04ClientMetadata(
    private val renderDistance: Byte,
    private val name: String
) : PacketPlayOut() {

    init {
        buffer = ByteBuffer.allocate(66).clear()

        buffer?.apply {
            order(ByteOrder.BIG_ENDIAN)
            put(ID)
            put(renderDistance)
            name.fit(64).encodeToByteArray().forEach { put(it) }
        }
    }
}