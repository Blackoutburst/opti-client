package dev.blackoutburst.game.window.callbacks

import dev.blackoutburst.game.Main.chatOpen
import dev.blackoutburst.game.input.Keyboard
import dev.blackoutburst.game.ui.Chat
import org.lwjgl.glfw.GLFW
import org.lwjgl.glfw.GLFW.*
import org.lwjgl.glfw.GLFWKeyCallbackI

class KeyboardCallBack : GLFWKeyCallbackI {
    override fun invoke(window: Long, key: Int, scancode: Int, action: Int, mods: Int) {
        Keyboard.keys[key] = action

        if (chatOpen && (action != 0))
            Chat.updateInput(key)
    }
}
