package dev.blackoutburst.game.window.nuklear.callbacks

import dev.blackoutburst.game.utils.stack
import dev.blackoutburst.game.window.nuklear.NK
import org.lwjgl.glfw.GLFWScrollCallbackI
import org.lwjgl.nuklear.NkVec2
import org.lwjgl.nuklear.Nuklear

class NuklearMouseScrollCallback: GLFWScrollCallbackI {
    override fun invoke(window: Long, xoffset: Double, yoffset: Double) {
        stack { stack ->
            val scroll = NkVec2.malloc(stack)
                .x(xoffset.toFloat())
                .y(yoffset.toFloat())
            Nuklear.nk_input_scroll(NK.ctx, scroll)
        }
    }
}