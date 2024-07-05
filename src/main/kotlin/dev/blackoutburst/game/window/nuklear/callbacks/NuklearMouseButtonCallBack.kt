package dev.blackoutburst.game.window.nuklear.callbacks

import dev.blackoutburst.game.utils.stack
import dev.blackoutburst.game.window.nuklear.NK
import org.lwjgl.glfw.GLFW
import org.lwjgl.glfw.GLFWMouseButtonCallbackI
import org.lwjgl.nuklear.Nuklear

class NuklearMouseButtonCallBack : GLFWMouseButtonCallbackI {
    override fun invoke(window: Long, button: Int, action: Int, mods: Int) {
        stack { stack ->
            val cx = stack.mallocDouble(1)
            val cy = stack.mallocDouble(1)

            GLFW.glfwGetCursorPos(window, cx, cy)

            val x = cx[0].toInt()
            val y = cy[0].toInt()
            val nkButton = when (button) {
                GLFW.GLFW_MOUSE_BUTTON_RIGHT -> Nuklear.NK_BUTTON_RIGHT
                GLFW.GLFW_MOUSE_BUTTON_MIDDLE -> Nuklear.NK_BUTTON_MIDDLE
                else -> Nuklear.NK_BUTTON_LEFT
            }
            Nuklear.nk_input_button(NK.ctx, nkButton, x, y, action == GLFW.GLFW_PRESS)
        }
    }
}
