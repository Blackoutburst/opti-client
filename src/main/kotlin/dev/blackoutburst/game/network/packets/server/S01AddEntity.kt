package dev.blackoutburst.game.network.packets.server

import dev.blackoutburst.game.entity.EntityManager
import dev.blackoutburst.game.entity.EntityOtherPlayer
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.network.packets.PacketPlayIn
import dev.blackoutburst.game.utils.SkinAPI
import dev.blackoutburst.game.utils.main
import java.nio.ByteBuffer
import java.nio.charset.StandardCharsets

class S01AddEntity(override val size: Int) : PacketPlayIn() {

    override fun decode(buffer: ByteBuffer) {
        val entityId = buffer.getInt()
        val x = buffer.getFloat()
        val y = buffer.getFloat()
        val z = buffer.getFloat()
        val yaw = buffer.getFloat()
        val pitch = buffer.getFloat()

        val data = mutableListOf<Byte>()
        for (i in 0 until 64) {
            data.add(buffer.get())
        }

        val buff = ByteBuffer.wrap(data.toByteArray())
        val name = StandardCharsets.UTF_8.decode(buff).toString().replace("\u0000", "")

        SkinAPI.load(name)

        main {
            EntityManager.addEntity(
                EntityOtherPlayer(
                    entityId,
                    Vector3f(x, y, z),
                    Vector2f(yaw, pitch),
                    name
                )
            )
        }
    }
}