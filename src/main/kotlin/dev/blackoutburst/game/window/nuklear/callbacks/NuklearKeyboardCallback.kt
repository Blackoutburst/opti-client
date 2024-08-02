package dev.blackoutburst.game.window.nuklear.callbacks

import dev.blackoutburst.game.window.nuklear.NK
import org.lwjgl.glfw.GLFW
import org.lwjgl.glfw.GLFWKeyCallbackI
import org.lwjgl.nuklear.Nuklear

class NuklearKeyboardCallback : GLFWKeyCallbackI {
    override fun invoke(window: Long, key: Int, scancode: Int, action: Int, mods: Int) {
        val press = action == GLFW.GLFW_PRESS
        when (key) {
            GLFW.GLFW_KEY_DELETE -> Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_DEL, press)
            GLFW.GLFW_KEY_ENTER -> Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_ENTER, press)
            GLFW.GLFW_KEY_TAB -> Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_TAB, press)
            GLFW.GLFW_KEY_BACKSPACE -> Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_BACKSPACE, press)
            GLFW.GLFW_KEY_UP -> Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_UP, press)
            GLFW.GLFW_KEY_DOWN -> Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_DOWN, press)
            GLFW.GLFW_KEY_HOME -> {
                Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_TEXT_START, press)
                Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_SCROLL_START, press)
            }

            GLFW.GLFW_KEY_END -> {
                Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_TEXT_END, press)
                Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_SCROLL_END, press)
            }

            GLFW.GLFW_KEY_PAGE_DOWN -> Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_SCROLL_DOWN, press)
            GLFW.GLFW_KEY_PAGE_UP -> Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_SCROLL_UP, press)
            GLFW.GLFW_KEY_LEFT_SHIFT, GLFW.GLFW_KEY_RIGHT_SHIFT -> Nuklear.nk_input_key(
                NK.ctx,
                Nuklear.NK_KEY_SHIFT,
                press
            )

            GLFW.GLFW_KEY_LEFT_CONTROL, GLFW.GLFW_KEY_RIGHT_CONTROL -> if (press) {
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_COPY,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_C) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_PASTE,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_P) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_CUT,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_X) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_TEXT_UNDO,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_Z) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_TEXT_REDO,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_R) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_TEXT_WORD_LEFT,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_LEFT) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_TEXT_WORD_RIGHT,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_RIGHT) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_TEXT_LINE_START,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_B) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_TEXT_LINE_END,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_E) == GLFW.GLFW_PRESS
                )
            } else {
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_LEFT,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_LEFT) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(
                    NK.ctx,
                    Nuklear.NK_KEY_RIGHT,
                    GLFW.glfwGetKey(window, GLFW.GLFW_KEY_RIGHT) == GLFW.GLFW_PRESS
                )
                Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_COPY, false)
                Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_PASTE, false)
                Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_CUT, false)
                Nuklear.nk_input_key(NK.ctx, Nuklear.NK_KEY_SHIFT, false)
            }
        }
    }
}