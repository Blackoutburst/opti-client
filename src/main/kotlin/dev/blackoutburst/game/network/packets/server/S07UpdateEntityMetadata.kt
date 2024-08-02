package dev.blackoutburst.game.network.packets.server

import dev.blackoutburst.game.entity.EntityManager
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.network.packets.PacketPlayIn
import java.nio.ByteBuffer
import java.nio.charset.StandardCharsets

class S07UpdateEntityMetadata(override val size: Int) : PacketPlayIn() {

    override fun decode(buffer: ByteBuffer) {
        val entityId = buffer.getInt()

        val data = mutableListOf<Byte>()
        for (i in 0 until 64) {
            data.add(buffer.get())
        }

        val buff = ByteBuffer.wrap(data.toByteArray())
        val name = StandardCharsets.UTF_8.decode(buff).toString()

        EntityManager.setName(entityId, name)
    }
}