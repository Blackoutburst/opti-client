package dev.blackoutburst.game

import dev.blackoutburst.game.Main.blockType
import dev.blackoutburst.game.Main.chatOpen
import dev.blackoutburst.game.Main.queue
import dev.blackoutburst.game.Main.source
import dev.blackoutburst.game.Main.source2
import dev.blackoutburst.game.camera.Camera
import dev.blackoutburst.game.entity.EntityManager
import dev.blackoutburst.game.input.Keyboard
import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.network.Connection
import dev.blackoutburst.game.network.packets.client.C04ClientMetadata
import dev.blackoutburst.game.ui.*
import dev.blackoutburst.game.utils.SkinAPI
import dev.blackoutburst.game.window.Window
import dev.blackoutburst.game.window.Window.id
import dev.blackoutburst.game.world.BlockType
import dev.blackoutburst.game.world.World
import org.lwjgl.BufferUtils
import org.lwjgl.glfw.GLFW.*
import org.lwjgl.openal.ALC
import org.lwjgl.opengl.GL11.*
import org.lwjgl.openal.AL
import org.lwjgl.openal.AL10.*
import org.lwjgl.openal.ALC10.*
import org.lwjgl.system.MemoryUtil
import java.nio.ByteBuffer
import java.nio.IntBuffer
import java.util.concurrent.ConcurrentLinkedQueue
import javax.sound.sampled.AudioSystem

object Main {
    var chatOpen = false
    val queue: ConcurrentLinkedQueue<() -> Unit> = ConcurrentLinkedQueue()
    var fps = 0
    var source = 0
    var source2 = 0

    var blockType = BlockType.GRASS
}

fun main() {
    Window
    World


    Connection.open("162.19.137.231", 15000)
    //Connection.open("localhost", 15000)

    // AL SHITPOST

    val device = alcOpenDevice(null as? ByteBuffer?)
    val deviceCaps = ALC.createCapabilities(device)
    val context = alcCreateContext(device, null as? IntBuffer?)
    alcMakeContextCurrent(context)
    AL.createCapabilities(deviceCaps, MemoryUtil::memCallocPointer);

    val buffer = alGenBuffers()
    val waveFile = Main::class.java.getResourceAsStream("/awp.wav")
    val audioInputStream = AudioSystem.getAudioInputStream(waveFile)
    val format = if (audioInputStream.format.channels == 1) AL_FORMAT_MONO16 else AL_FORMAT_STEREO16
    val audioBytes = audioInputStream.readAllBytes()
    val audioBuffer = BufferUtils.createByteBuffer(audioBytes.size)
    audioBuffer.put(audioBytes).flip()
    alBufferData(buffer, format, audioBuffer, audioInputStream.format.sampleRate.toInt())

    source = alGenSources()
    alSourcei(source, AL_BUFFER, buffer)

    val buffer2 = alGenBuffers()
    val waveFile2 = Main::class.java.getResourceAsStream("/bomb.wav")
    val audioInputStream2 = AudioSystem.getAudioInputStream(waveFile2)
    val format2 = if (audioInputStream2.format.channels == 1) AL_FORMAT_MONO16 else AL_FORMAT_STEREO16
    val audioBytes2 = audioInputStream2.readAllBytes()
    val audioBuffer2 = BufferUtils.createByteBuffer(audioBytes2.size)
    audioBuffer2.put(audioBytes2).flip()
    alBufferData(buffer2, format2, audioBuffer2, audioInputStream2.format.sampleRate.toInt())

    source2 = alGenSources()
    alSourcei(source2, AL_BUFFER, buffer2)

    //AL SHITPOST END

    while (Window.isOpen) {
        while(queue.isNotEmpty()) queue.poll()?.invoke()
        Window.clear()

        if (Keyboard.isKeyPressed(GLFW_KEY_0))
            blockType = if (Keyboard.isKeyDown(GLFW_KEY_LEFT_ALT)) BlockType.OAK_PLANKS else BlockType.ERROR
        if (Keyboard.isKeyPressed(GLFW_KEY_1))
            blockType = if (Keyboard.isKeyDown(GLFW_KEY_LEFT_ALT)) BlockType.STONE_BRICKS else BlockType.GRASS
        if (Keyboard.isKeyPressed(GLFW_KEY_2))
            blockType = if (Keyboard.isKeyDown(GLFW_KEY_LEFT_ALT)) BlockType.NETHERRACK else BlockType.DIRT
        if (Keyboard.isKeyPressed(GLFW_KEY_3))
            blockType = if (Keyboard.isKeyDown(GLFW_KEY_LEFT_ALT)) BlockType.GOLD_BLOCK else BlockType.STONE
        if (Keyboard.isKeyPressed(GLFW_KEY_4))
            blockType = if (Keyboard.isKeyDown(GLFW_KEY_LEFT_ALT)) BlockType.PACKED_ICE else BlockType.OAK_LOG
        if (Keyboard.isKeyPressed(GLFW_KEY_5))
            blockType = if (Keyboard.isKeyDown(GLFW_KEY_LEFT_ALT)) BlockType.LAVA else BlockType.OAK_LEAVES
        if (Keyboard.isKeyPressed(GLFW_KEY_6))
            blockType = if (Keyboard.isKeyDown(GLFW_KEY_LEFT_ALT)) BlockType.BARREL else BlockType.GLASS
        if (Keyboard.isKeyPressed(GLFW_KEY_7))
            blockType = if (Keyboard.isKeyDown(GLFW_KEY_LEFT_ALT)) BlockType.BOOKSHELF else BlockType.WATER
        if (Keyboard.isKeyPressed(GLFW_KEY_8))
            blockType = BlockType.SAND
        if (Keyboard.isKeyPressed(GLFW_KEY_9))
            blockType = BlockType.SNOW

        if (!chatOpen && Keyboard.isKeyPressed(GLFW_KEY_T)) {
            glfwSetInputMode(id, GLFW_CURSOR, GLFW_CURSOR_NORMAL)
            chatOpen = true
        }

        glPolygonMode(GL_FRONT_AND_BACK, Render.renderMode)
        EntityManager.update()
        EntityManager.render(Camera.view, Matrix().projectionMatrix(90f, 1000f, 0.1f))


        World.render()

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
        glEnable(GL_BLEND)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glDisable(GL_DEPTH_TEST)

        Cursor.render()

        SystemUsage.render(0f, 0f, 130f, 70f)
        WorldInformation.render(130f, 0f, 130f, 140f)
        ConnectionStatus.render(0f, 70f, 130f, 70f)
        Position.render(260f, 0f, 130f, 140f)
        Render.render(390f, 0f, 130f, 140f)

        Chat.renderTextField()
        Chat.renderMessages()

        Window.update()
    }

    alcDestroyContext(context)
    alcCloseDevice(device)

    Connection.close()
    Window.destroy()
}