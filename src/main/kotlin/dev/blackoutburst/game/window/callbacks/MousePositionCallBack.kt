package dev.blackoutburst.game.window.callbacks

import dev.blackoutburst.game.input.Mouse
import dev.blackoutburst.game.utils.stack
import org.lwjgl.glfw.GLFW.glfwGetCursorPos
import org.lwjgl.glfw.GLFWCursorPosCallbackI

class MousePositionCallBack : GLFWCursorPosCallbackI {
    override fun invoke(window: Long, xPos: Double, yPos: Double) {
        stack {
            val width = it.mallocDouble(1)
            val height = it.mallocDouble(1)

            glfwGetCursorPos(window, width, height)

            Mouse.position.set(width.get().toFloat(), height.get().toFloat())
        }
    }
}
