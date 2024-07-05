package dev.blackoutburst.game.window.nuklear.callbacks

import dev.blackoutburst.game.window.nuklear.NK
import org.lwjgl.glfw.GLFWCursorPosCallbackI
import org.lwjgl.nuklear.Nuklear

class NuklearMousePositionCallback : GLFWCursorPosCallbackI {
    override fun invoke(window: Long, xpos: Double, ypos: Double) {
        Nuklear.nk_input_motion(NK.ctx, xpos.toInt(), ypos.toInt())
    }
}