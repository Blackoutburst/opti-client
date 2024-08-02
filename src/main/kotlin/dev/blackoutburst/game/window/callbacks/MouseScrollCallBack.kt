package dev.blackoutburst.game.window.callbacks

import dev.blackoutburst.game.Main.chatOpen
import dev.blackoutburst.game.input.Mouse
import dev.blackoutburst.game.ui.Chat
import org.lwjgl.glfw.GLFWScrollCallbackI

class MouseScrollCallBack : GLFWScrollCallbackI {
    override fun invoke(window: Long, xOffset: Double, yOffset: Double) {
        if (chatOpen) {
            Chat.scroll += yOffset.toInt()
            if (Chat.scroll < 0) Chat.scroll = 0
            if (Chat.scroll > Chat.messages.size - 14) Chat.scroll = Chat.messages.size - 14
        }

        Mouse.scroll = (Mouse.scroll + yOffset).toFloat()
    }
}
