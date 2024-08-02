package dev.blackoutburst.game.ui

import dev.blackoutburst.game.Main.chatOpen
import dev.blackoutburst.game.network.Connection
import dev.blackoutburst.game.network.packets.client.C03Chat
import dev.blackoutburst.game.utils.Color
import dev.blackoutburst.game.window.Window
import dev.blackoutburst.game.window.Window.id
import org.lwjgl.glfw.GLFW.*
import java.util.*
import kotlin.concurrent.schedule

object Chat {
    private val sentHistory = mutableListOf<String>()
    private val inputField = ColoredBox(0f,0f, 500f, 30f, Color(0f,0f,0f,0.4f))
    private val messagesField = ColoredBox(0f,30f, 500f, 300f, Color(0f,0f,0f,0.4f))
    private var inputText = Text(1f, 5f, 20f, "")
    private var currentInputText = ""
    private val tick = Text(1f, 5f, 20f, "_")
    private var showTick = true
    private var historyPosition = sentHistory.size

    var scroll = 0
    val messages = mutableListOf<Text>()

    fun addMessage(text: String) {
        messages.addFirst(Text(1f, 40f, 20f, text))
    }

    init {
        Timer().schedule(0, 500) {
            if (!Window.isOpen) this.cancel()
            showTick = !showTick
        }
    }

    fun updateInput(code: Int) {
        if (code == 265 && historyPosition > 0) {
            if (historyPosition == sentHistory.size) {
                currentInputText = inputText.text
            }

            inputText = Text(1f, 5f, 20f, sentHistory[--historyPosition])
        }

        if (code == 264 && historyPosition != sentHistory.size) historyPosition++

        if (code == 264 && historyPosition == sentHistory.size) {
            inputText = Text(1f, 5f, 20f, currentInputText)
        }
        else if (code == 264 && historyPosition < sentHistory.size) {
            inputText = Text(1f, 5f, 20f, sentHistory[historyPosition])
        }

        if (code == 257) {
            if (inputText.text.isNotEmpty()) {
                Connection.write(C03Chat(inputText.text))
                sentHistory.add(inputText.text)
            }
            clearInput()
        }

        if (code >= 20 && code < 127 && inputText.text.length < 256) {
            val str = inputText.text + Char(code)
            inputText = Text(1f, 5f, 20f, str)
        }

        if (code == 259 && inputText.text.isNotEmpty()) {
            val str = inputText.text.substring(0, inputText.text.length - 1)
            inputText = Text(1f, 5f, 20f, str)
        }
    }

    fun clearInput() {
        chatOpen = false
        currentInputText = ""
        inputText = Text(1f, 5f, 20f, "")
        historyPosition = sentHistory.size

        glfwSetInputMode(id, GLFW_CURSOR, GLFW_CURSOR_DISABLED)
    }

    fun renderTextField() {
        inputField.render()
        inputText.render()

        tick.x = (inputText.text.length * 10f) + 1f
        if (chatOpen && showTick)
            tick.render()


        inputField.color = Color(0f, 0f, 0f,  if (chatOpen) 0.7f else 0.4f)
    }

    fun renderMessages() {
        messagesField.render()

        for (i in 0 + scroll until 14 + scroll) {
            val msg = try { messages[i] } catch (ignored: Exception) { null } ?: continue
            msg.y = 40f + (20f * (i - scroll))
            msg.render()
        }
    }
}