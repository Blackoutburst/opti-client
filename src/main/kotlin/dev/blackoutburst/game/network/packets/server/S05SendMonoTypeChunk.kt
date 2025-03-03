package dev.blackoutburst.game.network.packets.server

import dev.blackoutburst.game.maths.Vector3i
import dev.blackoutburst.game.network.packets.PacketPlayIn
import dev.blackoutburst.game.world.World
import java.nio.ByteBuffer

class S05SendMonoTypeChunk(override val size: Int) : PacketPlayIn() {

    override fun decode(buffer: ByteBuffer) {
        val x = buffer.getInt()
        val y = buffer.getInt()
        val z = buffer.getInt()
        val type = buffer.get()
        val position = Vector3i(x, y, z)

        World.addChunk(position, Array(4096) { type }.toByteArray())
    }
}