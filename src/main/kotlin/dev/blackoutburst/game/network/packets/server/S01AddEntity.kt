package dev.blackoutburst.game.network.packets.server

import dev.blackoutburst.game.entity.EntityManager
import dev.blackoutburst.game.entity.EntityOtherPlayer
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.network.packets.PacketPlayIn
import dev.blackoutburst.game.utils.main
import java.nio.ByteBuffer

class S01AddEntity(override val size: Int) : PacketPlayIn() {

    override fun decode(buffer: ByteBuffer) {
        val entityId = buffer.getInt()
        val x = buffer.getFloat()
        val y = buffer.getFloat()
        val z = buffer.getFloat()
        val yaw = buffer.getFloat()
        val pitch = buffer.getFloat()

        main {
            EntityManager.addEntity(
                EntityOtherPlayer(
                    entityId,
                    Vector3f(x, y, z),
                    Vector2f(yaw, pitch)
                )
            )
        }
    }
}