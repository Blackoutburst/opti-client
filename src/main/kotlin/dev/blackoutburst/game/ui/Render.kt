package dev.blackoutburst.game.ui

import dev.blackoutburst.game.camera.Camera
import dev.blackoutburst.game.utils.stack
import dev.blackoutburst.game.window.nuklear.NK.ctx
import org.lwjgl.nuklear.NkRect
import org.lwjgl.nuklear.Nuklear.*
import org.lwjgl.opengl.GL11.*

object Render {

    var renderMode = GL_FILL

    fun render(x: Float, y: Float, width: Float, height: Float) {
        stack { stack ->
            val rect = NkRect.malloc(stack)
            if (nk_begin(
                    ctx,
                    "Render",
                    nk_rect(x, y, width, height, rect),
                    NK_WINDOW_BORDER or NK_WINDOW_MOVABLE or NK_WINDOW_MINIMIZABLE or NK_WINDOW_TITLE or NK_WINDOW_NO_SCROLLBAR
                )
            ) {
                nk_layout_row_dynamic(ctx, 20f, 1)
                if (nk_option_label(ctx, "Fill", renderMode == GL_FILL)) {
                    renderMode = GL_FILL
                }
                nk_layout_row_dynamic(ctx, 20f, 1)
                if (nk_option_label(ctx, "Line", renderMode == GL_LINE)) {
                    renderMode = GL_LINE
                }
                nk_layout_row_dynamic(ctx, 20f, 1)
                if (nk_option_label(ctx, "Point", renderMode == GL_POINT)) {
                    renderMode = GL_POINT
                }
            }
            nk_end(ctx)
        }
    }
}