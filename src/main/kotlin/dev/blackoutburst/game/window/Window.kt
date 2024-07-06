package dev.blackoutburst.game.window

import dev.blackoutburst.game.Main
import dev.blackoutburst.game.input.Keyboard
import dev.blackoutburst.game.input.Mouse
import dev.blackoutburst.game.maths.Vector2i
import dev.blackoutburst.game.utils.IOUtils
import dev.blackoutburst.game.utils.Time
import dev.blackoutburst.game.utils.stack
import dev.blackoutburst.game.window.callbacks.*
import dev.blackoutburst.game.window.nuklear.NK
import dev.blackoutburst.game.window.nuklear.callbacks.*
import org.lwjgl.Version
import org.lwjgl.glfw.*
import org.lwjgl.glfw.Callbacks.glfwFreeCallbacks
import org.lwjgl.glfw.GLFW.*
import org.lwjgl.nuklear.Nuklear
import org.lwjgl.opengl.GL.createCapabilities
import org.lwjgl.opengl.GL11.*
import org.lwjgl.stb.STBImage
import org.lwjgl.system.MemoryStack
import org.lwjgl.system.MemoryUtil.NULL
import org.lwjgl.system.Platform
import java.nio.ByteBuffer
import java.security.Key
import kotlin.random.Random
import kotlin.system.exitProcess

object Window {
    val title = MemoryStack.stackPush().UTF8("OpenGL [LWJGL - ${Version.getVersion()}]")
    var id = -1L
    var isOpen = false

    var renderPasses = 0
    var lastTime = glfwGetTime()

    val width: Int
        get() = getFrameBufferSize().x
    val height: Int
        get() = getFrameBufferSize().y

    init {
        glfwInit()
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)

        id = glfwCreateWindow(1280, 720, title, NULL, NULL)
        if (id == -1L) exitProcess(-1)

        glfwMakeContextCurrent(id)
        createCapabilities()

        glfwSwapInterval(if (Platform.get() == Platform.MACOSX) GLFW_TRUE else GLFW_FALSE)
        glfwSetInputMode(id, GLFW_CURSOR, GLFW_CURSOR_DISABLED)

        NK.init(id)

        glClearColor(Random.nextFloat(), Random.nextFloat(), Random.nextFloat(), 1f)

        setCallbacks()
        setIcons()

        isOpen = true
    }

    fun update() {
        isOpen = !(glfwWindowShouldClose(id) || Keyboard.isKeyPressed(GLFW_KEY_ESCAPE))
        Time.updateDelta()
        Mouse.update()
        Keyboard.update()

        renderPasses++
        if (Time.runtime - lastTime >= 1) {
            Main.fps = renderPasses
            renderPasses = 0
            lastTime = Time.runtime
        }

        NK.newFrame(id)
        NK.render(Nuklear.NK_ANTI_ALIASING_ON)
        glfwSwapBuffers(id)
        glfwPollEvents()
    }

    fun clear() {
        glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT)
        glEnable(GL_CULL_FACE)
        glEnable(GL_DEPTH_TEST)
    }

    fun destroy() {
        NK.shutdown()
        glfwFreeCallbacks(id)
        glfwDestroyWindow(id)
        glfwTerminate()
    }

    private fun setCallbacks() {
        val windowCallback = WindowCallBack()
        val nkMouseScroll = NuklearMouseScrollCallback()
        val mouseScroll = MouseScrollCallBack()
        val nkKey = NuklearKeyboardCallback()
        val _key = KeyboardCallBack()
        val nkMousePos = NuklearMousePositionCallback()
        val mousePos = MousePositionCallBack()
        val nkMouseButton = NuklearMouseButtonCallBack()
        val mouseButton = MouseButtonCallBack()

        glfwSetWindowSizeCallback(id, windowCallback)
        glfwSetScrollCallback(id) { window: Long, xoffset: Double, yoffset: Double ->
            nkMouseScroll.invoke(window, xoffset, yoffset)
            mouseScroll.invoke(window, xoffset, yoffset)
        }
        glfwSetCharCallback(id, NuklearCharCallback())
        glfwSetKeyCallback(id) { window: Long, key: Int, scancode: Int, action: Int, mods: Int ->
            nkKey.invoke(window, key, scancode, action, mods)
            _key.invoke(window, key, scancode, action, mods)
        }

        glfwSetCursorPosCallback(id) { window: Long, xpos: Double, ypos: Double ->
            nkMousePos.invoke(window, xpos, ypos)
            mousePos.invoke(window, xpos, ypos)
        }

        glfwSetMouseButtonCallback(id) { window: Long, button: Int, action: Int, mods: Int ->
            nkMouseButton.invoke(window, button, action, mods)
            mouseButton.invoke(window, button, action, mods)
        }
    }

    private fun setIcons() {
        if (Platform.get() == Platform.MACOSX) return

        val image = GLFWImage.malloc()
        val buffer = GLFWImage.malloc(1)
        try {
            image[64, 64] = loadIcon()
            buffer.put(0, image)
            glfwSetWindowIcon(id, buffer)
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    @Throws(Exception::class)
    private fun loadIcon(): ByteBuffer {
        var image: ByteBuffer? = null

        stack {
            val comp = it.mallocInt(1)
            val w = it.mallocInt(1)
            val h = it.mallocInt(1)
            val img = IOUtils.ioResourceToByteBuffer("icon.png", 8 * 1024)

            image = STBImage.stbi_load_from_memory(img, w, h, comp, 4)
            if (image == null) {
                throw Exception("Failed to load icons")
            }
        }

        return image!!
    }

    private fun getFrameBufferSize(): Vector2i {
        val size = Vector2i()
        stack {
            val width = it.mallocInt(1)
            val height = it.mallocInt(1)

            glfwGetFramebufferSize(id, width, height)
            size.set(width[0], height[0])
        }

        return size
    }
}