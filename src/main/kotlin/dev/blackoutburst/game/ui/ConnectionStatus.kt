package dev.blackoutburst.game.ui

import dev.blackoutburst.game.network.Connection
import dev.blackoutburst.game.utils.stack
import dev.blackoutburst.game.window.nuklear.NK.ctx
import dev.blackoutburst.game.world.World
import org.lwjgl.nuklear.NkRect
import org.lwjgl.nuklear.Nuklear.*

object ConnectionStatus {
    fun render(x: Float, y: Float, width: Float, height: Float) {
        stack { stack ->
            val rect = NkRect.malloc(stack)
            if (nk_begin(
                    ctx,
                    "Connection",
                    nk_rect(x, y, width, height, rect),
                    NK_WINDOW_BORDER or NK_WINDOW_MOVABLE or NK_WINDOW_MINIMIZABLE or NK_WINDOW_TITLE or NK_WINDOW_NO_SCROLLBAR
                )
            ) {
                nk_layout_row_dynamic(ctx, 20f, 1)
                nk_label(ctx, "Connected: ${Connection.isOpen}", NK_TEXT_LEFT)
            }
            nk_end(ctx)
        }
    }
}