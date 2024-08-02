package dev.blackoutburst.game.ui

import dev.blackoutburst.game.input.Keyboard
import dev.blackoutburst.game.network.Connection
import dev.blackoutburst.game.network.packets.client.C03Chat
import dev.blackoutburst.game.utils.stack
import dev.blackoutburst.game.window.nuklear.NK.ctx
import org.lwjgl.glfw.GLFW
import org.lwjgl.nuklear.NkPluginFilterI
import org.lwjgl.nuklear.NkRect
import org.lwjgl.nuklear.Nuklear.*
import org.lwjgl.system.MemoryUtil
import java.nio.charset.StandardCharsets

class DefaultFilter : NkPluginFilterI {
    override fun invoke(p0: Long, p1: Int): Boolean = nnk_filter_ascii(p0, p1)
}

object Chat {
    val messages = mutableListOf<String>()
    val text = MemoryUtil.memAlloc(4096).put(Array(4095) { 0.toByte() }.toByteArray()).flip()
    var flags = 0

    var totalHeight = 0

    fun clear() {
        val msg = StandardCharsets.UTF_8.decode(text).toString()
        Connection.write(C03Chat(msg))
        text.clear().put(Array(4095) { 0.toByte() }.toByteArray()).flip()

        totalHeight = messages.size * 30

        nk_window_set_scroll(ctx, 0, totalHeight + 200)
    }

    fun renderTextField(x: Float, y: Float, width: Float, height: Float) {

        stack { stack ->

            ctx.style().window().fixed_background().data().color().set(10.toByte(), 10.toByte(), 10.toByte(), 150.toByte())
            ctx.style().edit().border_color().set(100.toByte(), 100.toByte(), 100.toByte(), 255.toByte())
            ctx.style().edit().border(1f)
            ctx.style().edit().normal().data().color().set(0.toByte(), 0.toByte(), 0.toByte(), 0.toByte())
            ctx.style().edit().hover().data().color().set(0.toByte(), 0.toByte(), 0.toByte(), 50.toByte())
            ctx.style().edit().active().data().color().set(0.toByte(), 0.toByte(), 0.toByte(), 100.toByte())

            val rect = NkRect.malloc(stack)
            if (nk_begin(
                    ctx,
                    "Chat Input",
                    nk_rect(x, y, width, height, rect),
                    NK_WINDOW_NO_SCROLLBAR
                )
            ) {

                nk_layout_row_dynamic(ctx, 30f, 1)
                flags = nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD or NK_EDIT_SELECTABLE, text, 4096, DefaultFilter())
            }
            nk_end(ctx)
        }

        ctx.style().window().fixed_background().data().color().set(50.toByte(), 50.toByte(), 50.toByte(), 255.toByte())
    }

    fun renderMessages(x: Float, y: Float, width: Float, height: Float) {
        stack { stack ->
            ctx.style().window().fixed_background().data().color().set(10.toByte(), 10.toByte(), 10.toByte(), 150.toByte())

            val rect = NkRect.malloc(stack)
            if (nk_begin(
                    ctx,
                    "Chat Messages",
                    nk_rect(x, y, width, height, rect),
                    0
                )
            ) {

                val size = messages.size
                for (i in 0 until size) {
                    val msg = try { messages[i] } catch (ignored: Exception) { null } ?: continue

                    nk_layout_row_dynamic(ctx, 20f, 1)
                    nk_label(ctx, msg, NK_TEXT_LEFT)
                }

                if (flags and NK_EDIT_ACTIVE != 0 && Keyboard.isKeyPressed(GLFW.GLFW_KEY_ENTER)) {
                    clear()
                }
            }
            nk_end(ctx)
        }

        ctx.style().window().fixed_background().data().color().set(50.toByte(), 50.toByte(), 50.toByte(), 255.toByte())
    }
}