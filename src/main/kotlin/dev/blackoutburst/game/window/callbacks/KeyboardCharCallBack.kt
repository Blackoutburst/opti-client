package dev.blackoutburst.game.window.callbacks

import dev.blackoutburst.game.Main.chatOpen
import dev.blackoutburst.game.ui.Chat
import org.lwjgl.glfw.GLFWCharCallbackI

class KeyboardCharCallBack : GLFWCharCallbackI {
    override fun invoke(window: Long, key: Int) {
        if (chatOpen)
            Chat.updateInput(key)
    }
}
