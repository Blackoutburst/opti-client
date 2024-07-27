package dev.blackoutburst.game.network.packets.server

import dev.blackoutburst.game.entity.EntityPlayer
import dev.blackoutburst.game.entity.EntityManager
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.network.Connection
import dev.blackoutburst.game.network.packets.PacketPlayIn
import dev.blackoutburst.game.ui.Chat
import dev.blackoutburst.game.utils.main
import java.nio.ByteBuffer
import java.nio.charset.StandardCharsets

class S06Chat(override val size: Int): PacketPlayIn() {

    override fun decode(buffer: ByteBuffer) {
        val data = mutableListOf<Byte>()
        for (i in 0 until 4096) {
            data.add(buffer.get())
        }

        val buff = ByteBuffer.wrap(data.toByteArray())

        Chat.messages.add(StandardCharsets.UTF_8.decode(buff).toString())
    }
}