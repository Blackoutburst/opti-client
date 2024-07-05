package dev.blackoutburst.game.network.packets.server

import dev.blackoutburst.game.entity.EntityPlayer
import dev.blackoutburst.game.entity.EntityManager
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.network.Connection
import dev.blackoutburst.game.network.packets.PacketPlayIn
import dev.blackoutburst.game.utils.main
import java.nio.ByteBuffer

class S00Identification(override val size: Int): PacketPlayIn() {

    override fun decode(buffer: ByteBuffer) {
        val entityId = buffer.getInt()
        main {
            EntityManager.addEntity(
                EntityPlayer(
                    id = entityId,
                    position = Vector3f(),
                )
            )
            Connection.id = entityId
        }
    }
}