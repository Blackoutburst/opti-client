package dev.blackoutburst.game.window.nuklear.callbacks

import dev.blackoutburst.game.window.nuklear.NK
import org.lwjgl.glfw.GLFWCharCallbackI
import org.lwjgl.nuklear.Nuklear

class NuklearCharCallback: GLFWCharCallbackI {
    override fun invoke(window: Long, codepoint: Int) {
        Nuklear.nk_input_unicode(NK.ctx, codepoint);
    }
}