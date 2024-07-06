package dev.blackoutburst.game.ui

import dev.blackoutburst.game.camera.Camera
import dev.blackoutburst.game.utils.stack
import dev.blackoutburst.game.window.nuklear.NK.ctx
import org.lwjgl.nuklear.NkRect
import org.lwjgl.nuklear.Nuklear.*

object Position {
    fun render(x: Float, y: Float, width: Float, height: Float) {
        stack { stack ->
            val rect = NkRect.malloc(stack)
            if (nk_begin(
                    ctx,
                    "Position",
                    nk_rect(x, y, width, height, rect),
                    NK_WINDOW_BORDER or NK_WINDOW_MOVABLE or NK_WINDOW_MINIMIZABLE or NK_WINDOW_TITLE or NK_WINDOW_NO_SCROLLBAR
                )
            ) {
                nk_layout_row_dynamic(ctx, 20f, 1)
                nk_label(ctx, "X: ${Camera.position.x}", NK_TEXT_LEFT)
                nk_label(ctx, "Y: ${Camera.position.y}", NK_TEXT_LEFT)
                nk_label(ctx, "Z: ${Camera.position.z}", NK_TEXT_LEFT)
            }
            nk_end(ctx)
        }
    }
}