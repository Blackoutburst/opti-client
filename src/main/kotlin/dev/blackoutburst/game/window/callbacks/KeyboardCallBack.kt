package dev.blackoutburst.game.window.callbacks

import dev.blackoutburst.game.input.Keyboard
import org.lwjgl.glfw.GLFW
import org.lwjgl.glfw.GLFWKeyCallbackI

class KeyboardCallBack : GLFWKeyCallbackI {
    override fun invoke(window: Long, key: Int, scancode: Int, action: Int, mods: Int) {
        Keyboard.keys[key] = action
    }
}
