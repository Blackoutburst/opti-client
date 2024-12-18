package dev.blackoutburst.game.network.packets.server

import dev.blackoutburst.game.maths.Vector3i
import dev.blackoutburst.game.network.packets.PacketPlayIn
import dev.blackoutburst.game.world.World
import java.nio.ByteBuffer

class S04SendChunk(override val size: Int) : PacketPlayIn() {

    override fun decode(buffer: ByteBuffer) {
        val x = buffer.getInt()
        val y = buffer.getInt()
        val z = buffer.getInt()

        val position = Vector3i(x, y, z)
        val blockData = Array(4096) { buffer.get() }.toByteArray()

        World.addChunk(position, blockData)
    }
}