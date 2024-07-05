package dev.blackoutburst.game.network.packets.server

import dev.blackoutburst.game.entity.EntityManager
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.network.packets.PacketPlayIn
import java.nio.ByteBuffer

class S03UpdateEntity(override val size: Int) : PacketPlayIn() {

    override fun decode(buffer: ByteBuffer) {
        val entityId = buffer.getInt()
        val x = buffer.getFloat()
        val y = buffer.getFloat()
        val z = buffer.getFloat()
        val yaw = buffer.getFloat()
        val pitch = buffer.getFloat()

        EntityManager.setPosition(entityId, Vector3f(x, y, z))
        EntityManager.setRotation(entityId, Vector2f(yaw, pitch))
    }
}