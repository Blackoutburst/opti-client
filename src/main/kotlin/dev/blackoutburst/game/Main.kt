package dev.blackoutburst.game

import dev.blackoutburst.game.Main.queue
import dev.blackoutburst.game.entity.EntityManager
import dev.blackoutburst.game.network.Connection
import dev.blackoutburst.game.ui.ConnectionStatus
import dev.blackoutburst.game.ui.Position
import dev.blackoutburst.game.ui.SystemUsage
import dev.blackoutburst.game.ui.WorldInformation
import dev.blackoutburst.game.window.Window
import dev.blackoutburst.game.world.World
import java.util.concurrent.ConcurrentLinkedQueue

object Main {
    val queue: ConcurrentLinkedQueue<() -> Unit> = ConcurrentLinkedQueue()
    var fps = 0
}

fun main() {
    Window
    World

    Connection.open("localhost", 15000)

    while (Window.isOpen) {
        while(queue.isNotEmpty()) queue.poll()?.invoke()
        Window.clear()

        EntityManager.update()

        EntityManager.render()

        World.render()

        SystemUsage.render(0f, 0f, 130f, 140f)
        WorldInformation.render(130f, 0f, 130f, 70f)
        ConnectionStatus.render(130f, 70f, 130f, 70f)
        Position.render(260f, 0f, 130f, 140f)

        Window.update()
    }

    Connection.close()
    Window.destroy()
}