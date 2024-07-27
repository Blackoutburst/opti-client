package dev.blackoutburst.game.window.nuklear

import dev.blackoutburst.game.utils.IOUtils.ioResourceToByteBuffer
import org.lwjgl.glfw.GLFW
import org.lwjgl.nuklear.*
import org.lwjgl.opengl.*
import org.lwjgl.stb.*
import org.lwjgl.system.MemoryStack
import org.lwjgl.system.MemoryUtil
import org.lwjgl.system.Platform
import java.io.IOException
import java.nio.ByteBuffer
import java.util.*

object NK {
    var ctx: NkContext = NkContext.create()
    private val default_font: NkUserFont = NkUserFont.create()

    private val cmds: NkBuffer = NkBuffer.create()
    private val null_texture: NkDrawNullTexture = NkDrawNullTexture.create()

    private val ALLOCATOR: NkAllocator = NkAllocator.create()
        .alloc { handle: Long, old: Long, size: Long -> MemoryUtil.nmemAllocChecked(size) }
        .mfree { handle: Long, ptr: Long -> MemoryUtil.nmemFree(ptr) }

    private val VERTEX_LAYOUT: NkDrawVertexLayoutElement.Buffer = NkDrawVertexLayoutElement.create(4)
        .position(0).attribute(Nuklear.NK_VERTEX_POSITION).format(Nuklear.NK_FORMAT_FLOAT).offset(0)
        .position(1).attribute(Nuklear.NK_VERTEX_TEXCOORD).format(Nuklear.NK_FORMAT_FLOAT).offset(8)
        .position(2).attribute(Nuklear.NK_VERTEX_COLOR).format(Nuklear.NK_FORMAT_R8G8B8A8).offset(16)
        .position(3).attribute(Nuklear.NK_VERTEX_ATTRIBUTE_COUNT).format(Nuklear.NK_FORMAT_COUNT).offset(0)
        .flip()

    private val BUFFER_INITIAL_SIZE = 4 * 1024

    private val MAX_VERTEX_BUFFER = 512 * 1024
    private val MAX_ELEMENT_BUFFER = 128 * 1024

    private var width = 0
    private var height = 0

    private var display_width = 0
    private var display_height = 0
    private var vbo = 0
    private var vao = 0
    private var ebo = 0
    private var prog = 0
    private var vert_shdr = 0
    private var frag_shdr = 0
    private var uniform_tex = 0
    private var uniform_proj = 0

    private var ttf: ByteBuffer? = null

    init {
        try {
            this.ttf = ioResourceToByteBuffer("roboto.ttf", 512 * 1024)
        } catch (e: IOException) {
            throw RuntimeException(e)
        }
    }

    fun init(window: Long) {
        val ctx = setupWindow(window)

        val BITMAP_W = 1024
        val BITMAP_H = 1024

        val FONT_HEIGHT = 18
        val fontTexID = GL11C.glGenTextures()

        val fontInfo = STBTTFontinfo.create()
        val cdata = STBTTPackedchar.create(95)

        var scale: Float
        var descent: Float

        MemoryStack.stackPush().use { stack ->
            STBTruetype.stbtt_InitFont(fontInfo, ttf!!)
            scale = STBTruetype.stbtt_ScaleForPixelHeight(fontInfo, FONT_HEIGHT.toFloat())

            val d = stack.mallocInt(1)
            STBTruetype.stbtt_GetFontVMetrics(fontInfo, null, d, null)
            descent = d[0] * scale

            val bitmap = MemoryUtil.memAlloc(BITMAP_W * BITMAP_H)

            val pc = STBTTPackContext.malloc(stack)
            STBTruetype.stbtt_PackBegin(pc, bitmap, BITMAP_W, BITMAP_H, 0, 1, MemoryUtil.NULL)
            STBTruetype.stbtt_PackSetOversampling(pc, 4, 4)
            STBTruetype.stbtt_PackFontRange(pc, ttf!!, 0, FONT_HEIGHT.toFloat(), 32, cdata)
            STBTruetype.stbtt_PackEnd(pc)

            val texture = MemoryUtil.memAlloc(BITMAP_W * BITMAP_H * 4)
            for (i in 0 until bitmap.capacity()) {
                texture.putInt((bitmap[i].toInt() shl 24) or 0x00FFFFFF)
            }
            texture.flip()

            GL11.glBindTexture(GL11C.GL_TEXTURE_2D, fontTexID)
            GL11.glTexImage2D(
                GL11C.GL_TEXTURE_2D,
                0,
                GL11C.GL_RGBA8,
                BITMAP_W,
                BITMAP_H,
                0,
                GL11C.GL_RGBA,
                GL12C.GL_UNSIGNED_INT_8_8_8_8_REV,
                texture
            )
            GL11C.glTexParameteri(GL11C.GL_TEXTURE_2D, GL11C.GL_TEXTURE_MAG_FILTER, GL11C.GL_LINEAR)
            GL11C.glTexParameteri(GL11C.GL_TEXTURE_2D, GL11C.GL_TEXTURE_MIN_FILTER, GL11C.GL_LINEAR)

            MemoryUtil.memFree(texture)
            MemoryUtil.memFree(bitmap)
        }
        default_font
            .width { handle: Long, h: Float, text: Long, len: Int ->
                var text_width = 0f
                MemoryStack.stackPush().use { stack ->
                    val unicode = stack.mallocInt(1)
                    var glyph_len = Nuklear.nnk_utf_decode(text, MemoryUtil.memAddress(unicode), len)
                    var text_len = glyph_len

                    if (glyph_len == 0) {
                        return@width 0f
                    }

                    val advance = stack.mallocInt(1)
                    while (text_len <= len && glyph_len != 0) {
                        if (unicode[0] == Nuklear.NK_UTF_INVALID) {
                            break
                        }

                        /* query currently drawn glyph information */
                        STBTruetype.stbtt_GetCodepointHMetrics(fontInfo, unicode[0], advance, null)
                        text_width += advance[0] * scale

                        /* offset next glyph */
                        glyph_len =
                            Nuklear.nnk_utf_decode(text + text_len, MemoryUtil.memAddress(unicode), len - text_len)
                        text_len += glyph_len
                    }
                }
                text_width
            }
            .height(FONT_HEIGHT.toFloat())
            .query { handle: Long, font_height: Float, glyph: Long, codepoint: Int, next_codepoint: Int ->
                MemoryStack.stackPush().use { stack ->
                    val x = stack.floats(0.0f)
                    val y = stack.floats(0.0f)

                    val q = STBTTAlignedQuad.malloc(stack)
                    val advance = stack.mallocInt(1)

                    STBTruetype.stbtt_GetPackedQuad(cdata, BITMAP_W, BITMAP_H, codepoint - 32, x, y, q, false)
                    STBTruetype.stbtt_GetCodepointHMetrics(fontInfo, codepoint, advance, null)

                    val ufg = NkUserFontGlyph.create(glyph)

                    ufg.width(q.x1() - q.x0())
                    ufg.height(q.y1() - q.y0())
                    ufg.offset()[q.x0()] = q.y0() + (FONT_HEIGHT + descent)
                    ufg.xadvance(advance[0] * scale)
                    ufg.uv(0)[q.s0()] = q.t0()
                    ufg.uv(1)[q.s1()] = q.t1()
                }
            }
            .texture { it: NkHandle ->
                it
                    .id(fontTexID)
            }

        Nuklear.nk_style_set_font(ctx, default_font)
    }

    private fun setupContext() {
        val NK_SHADER_VERSION = if (Platform.get() === Platform.MACOSX) "#version 150\n" else "#version 300 es\n"
        val vertex_shader =
            """${NK_SHADER_VERSION}uniform mat4 ProjMtx;
in vec2 Position;
in vec2 TexCoord;
in vec4 Color;
out vec2 Frag_UV;
out vec4 Frag_Color;
void main() {
   Frag_UV = TexCoord;
   Frag_Color = Color;
   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
}
"""
        val fragment_shader =
            """${NK_SHADER_VERSION}precision mediump float;
uniform sampler2D Texture;
in vec2 Frag_UV;
in vec4 Frag_Color;
out vec4 Out_Color;
void main(){
   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
}
"""

        Nuklear.nk_buffer_init(cmds, ALLOCATOR, BUFFER_INITIAL_SIZE.toLong())
        prog = GL20C.glCreateProgram()
        vert_shdr = GL20C.glCreateShader(GL20C.GL_VERTEX_SHADER)
        frag_shdr = GL20C.glCreateShader(GL20C.GL_FRAGMENT_SHADER)
        GL20C.glShaderSource(vert_shdr, vertex_shader)
        GL20C.glShaderSource(frag_shdr, fragment_shader)
        GL20C.glCompileShader(vert_shdr)
        GL20C.glCompileShader(frag_shdr)
        check(GL20C.glGetShaderi(vert_shdr, GL20C.GL_COMPILE_STATUS) == GL11C.GL_TRUE)
        check(GL20C.glGetShaderi(frag_shdr, GL20C.GL_COMPILE_STATUS) == GL11C.GL_TRUE)
        GL20C.glAttachShader(prog, vert_shdr)
        GL20C.glAttachShader(prog, frag_shdr)
        GL20C.glLinkProgram(prog)
        check(GL20C.glGetProgrami(prog, GL20C.GL_LINK_STATUS) == GL11C.GL_TRUE)

        uniform_tex = GL20C.glGetUniformLocation(prog, "Texture")
        uniform_proj = GL20C.glGetUniformLocation(prog, "ProjMtx")
        val attrib_pos = GL20C.glGetAttribLocation(prog, "Position")
        val attrib_uv = GL20C.glGetAttribLocation(prog, "TexCoord")
        val attrib_col = GL20C.glGetAttribLocation(prog, "Color")

        run {
            // buffer setup
            vbo = GL15C.glGenBuffers()
            ebo = GL15C.glGenBuffers()
            vao = GL30C.glGenVertexArrays()

            GL30C.glBindVertexArray(vao)
            GL15C.glBindBuffer(GL15C.GL_ARRAY_BUFFER, vbo)
            GL15C.glBindBuffer(GL15C.GL_ELEMENT_ARRAY_BUFFER, ebo)

            GL20C.glEnableVertexAttribArray(attrib_pos)
            GL20C.glEnableVertexAttribArray(attrib_uv)
            GL20C.glEnableVertexAttribArray(attrib_col)

            GL20C.glVertexAttribPointer(attrib_pos, 2, GL11C.GL_FLOAT, false, 20, 0)
            GL20C.glVertexAttribPointer(attrib_uv, 2, GL11C.GL_FLOAT, false, 20, 8)
            GL20C.glVertexAttribPointer(attrib_col, 4, GL11C.GL_UNSIGNED_BYTE, true, 20, 16)
        }

        run {
            // null texture setup
            val nullTexID = GL11C.glGenTextures()

            null_texture.texture().id(nullTexID)
            null_texture.uv()[0.5f] = 0.5f

            GL11C.glBindTexture(GL11C.GL_TEXTURE_2D, nullTexID)
            MemoryStack.stackPush().use { stack ->
                GL11C.glTexImage2D(
                    GL11C.GL_TEXTURE_2D,
                    0,
                    GL11C.GL_RGBA8,
                    1,
                    1,
                    0,
                    GL11C.GL_RGBA,
                    GL12C.GL_UNSIGNED_INT_8_8_8_8_REV,
                    stack.ints(-0x1)
                )
            }
            GL11C.glTexParameteri(GL11C.GL_TEXTURE_2D, GL11C.GL_TEXTURE_MAG_FILTER, GL11C.GL_NEAREST)
            GL11C.glTexParameteri(GL11C.GL_TEXTURE_2D, GL11C.GL_TEXTURE_MIN_FILTER, GL11C.GL_NEAREST)
        }

        GL11C.glBindTexture(GL11C.GL_TEXTURE_2D, 0)
        GL15C.glBindBuffer(GL15C.GL_ARRAY_BUFFER, 0)
        GL15C.glBindBuffer(GL15C.GL_ELEMENT_ARRAY_BUFFER, 0)
        GL30C.glBindVertexArray(0)
    }

    private fun setupWindow(win: Long): NkContext {
        Nuklear.nk_init(ctx, ALLOCATOR, null)
        ctx.clip()
            .copy { handle: Long, text: Long, len: Int ->
                if (len == 0) {
                    return@copy
                }
                MemoryStack.stackPush().use { stack ->
                    val str = stack.malloc(len + 1)
                    MemoryUtil.memCopy(text, MemoryUtil.memAddress(str), len.toLong())
                    str.put(len, 0.toByte())
                    GLFW.glfwSetClipboardString(win, str)
                }
            }
            .paste { handle: Long, edit: Long ->
                val text = GLFW.nglfwGetClipboardString(win)
                if (text != MemoryUtil.NULL) {
                    Nuklear.nnk_textedit_paste(edit, text, Nuklear.nnk_strlen(text))
                }
            }

        setupContext()
        return ctx
    }

    fun newFrame(window: Long) {
        MemoryStack.stackPush().use { stack ->
            val w = stack.mallocInt(1)
            val h = stack.mallocInt(1)

            GLFW.glfwGetWindowSize(window, w, h)
            width = w[0]
            height = h[0]

            GLFW.glfwGetFramebufferSize(window, w, h)
            display_width = w[0]
            display_height = h[0]
        }
        Nuklear.nk_input_begin(ctx)

        val mouse = ctx.input().mouse()
        if (mouse.grab()) {
            GLFW.glfwSetInputMode(window, GLFW.GLFW_CURSOR, GLFW.GLFW_CURSOR_HIDDEN)
        } else if (mouse.grabbed()) {
            val prevX = mouse.prev().x()
            val prevY = mouse.prev().y()
            GLFW.glfwSetCursorPos(window, prevX.toDouble(), prevY.toDouble())
            mouse.pos().x(prevX)
            mouse.pos().y(prevY)
        } else if (mouse.ungrab()) {
            GLFW.glfwSetInputMode(window, GLFW.GLFW_CURSOR, GLFW.GLFW_CURSOR_NORMAL)
        }

        Nuklear.nk_input_end(ctx)
    }

    fun render(AA: Int) {
        MemoryStack.stackPush().use { stack ->
            // setup global state
            GL11C.glEnable(GL11C.GL_BLEND)
            GL14C.glBlendEquation(GL14C.GL_FUNC_ADD)
            GL11C.glBlendFunc(GL11C.GL_SRC_ALPHA, GL11C.GL_ONE_MINUS_SRC_ALPHA)
            GL11C.glDisable(GL11C.GL_CULL_FACE)
            GL11C.glDisable(GL11C.GL_DEPTH_TEST)
            GL11C.glEnable(GL11C.GL_SCISSOR_TEST)
            GL13C.glActiveTexture(GL13C.GL_TEXTURE0)

            // setup program
            GL20C.glUseProgram(prog)
            GL20C.glUniform1i(uniform_tex, 0)
            GL20C.glUniformMatrix4fv(
                uniform_proj, false, stack.floats(
                    2.0f / width, 0.0f, 0.0f, 0.0f,
                    0.0f, -2.0f / height, 0.0f, 0.0f,
                    0.0f, 0.0f, -1.0f, 0.0f,
                    -1.0f, 1.0f, 0.0f, 1.0f
                )
            )
            GL11C.glViewport(0, 0, display_width, display_height)
        }
        run {
            // convert from command queue into draw list and draw to screen
            // allocate vertex and element buffer
            GL30C.glBindVertexArray(vao)
            GL15C.glBindBuffer(GL15C.GL_ARRAY_BUFFER, vbo)
            GL15C.glBindBuffer(GL15C.GL_ELEMENT_ARRAY_BUFFER, ebo)

            GL15C.glBufferData(GL15C.GL_ARRAY_BUFFER, MAX_VERTEX_BUFFER.toLong(), GL15C.GL_STREAM_DRAW)
            GL15C.glBufferData(GL15C.GL_ELEMENT_ARRAY_BUFFER, MAX_ELEMENT_BUFFER.toLong(), GL15C.GL_STREAM_DRAW)

            // load draw vertices & elements directly into vertex + element buffer
            val vertices = Objects.requireNonNull(
                GL15C.glMapBuffer(
                    GL15C.GL_ARRAY_BUFFER,
                    GL15C.GL_WRITE_ONLY,
                    MAX_VERTEX_BUFFER.toLong(),
                    null
                )
            )
            val elements = Objects.requireNonNull(
                GL15C.glMapBuffer(
                    GL15C.GL_ELEMENT_ARRAY_BUFFER,
                    GL15C.GL_WRITE_ONLY,
                    MAX_ELEMENT_BUFFER.toLong(),
                    null
                )
            )
            MemoryStack.stackPush().use { stack ->
                // fill convert configuration
                val config = NkConvertConfig.calloc(stack)
                    .vertex_layout(VERTEX_LAYOUT)
                    .vertex_size(20)
                    .vertex_alignment(4)
                    .tex_null(null_texture)
                    .circle_segment_count(22)
                    .curve_segment_count(22)
                    .arc_segment_count(22)
                    .global_alpha(1.0f)
                    .shape_AA(AA)
                    .line_AA(AA)

                // setup buffers to load vertices and elements
                val vbuf = NkBuffer.malloc(stack)
                val ebuf = NkBuffer.malloc(stack)

                Nuklear.nk_buffer_init_fixed(vbuf, vertices!! /*, max_vertex_buffer*/)
                Nuklear.nk_buffer_init_fixed(ebuf, elements!! /*, max_element_buffer*/)
                Nuklear.nk_convert(ctx, cmds, vbuf, ebuf, config)
            }
            GL15C.glUnmapBuffer(GL15C.GL_ELEMENT_ARRAY_BUFFER)
            GL15C.glUnmapBuffer(GL15C.GL_ARRAY_BUFFER)

            // iterate over and execute each draw command
            val fb_scale_x = display_width.toFloat() / width.toFloat()
            val fb_scale_y = display_height.toFloat() / height.toFloat()

            var offset = MemoryUtil.NULL
            var cmd = Nuklear.nk__draw_begin(ctx, cmds)
            while (cmd != null) {
                if (cmd.elem_count() == 0) {
                    cmd = Nuklear.nk__draw_next(cmd, cmds, ctx)
                    continue
                }
                GL11C.glBindTexture(GL11C.GL_TEXTURE_2D, cmd.texture().id())
                GL11C.glScissor(
                    (cmd.clip_rect().x() * fb_scale_x).toInt(),
                    ((height - (cmd.clip_rect().y() + cmd.clip_rect().h()).toInt()) * fb_scale_y).toInt(),
                    (cmd.clip_rect().w() * fb_scale_x).toInt(),
                    (cmd.clip_rect().h() * fb_scale_y).toInt()
                )
                GL11C.glDrawElements(GL11C.GL_TRIANGLES, cmd.elem_count(), GL11C.GL_UNSIGNED_SHORT, offset)
                offset += cmd.elem_count() * 2L
                cmd = Nuklear.nk__draw_next(cmd, cmds, ctx)
            }
            Nuklear.nk_clear(ctx)
            Nuklear.nk_buffer_clear(cmds)
        }

        // default OpenGL state
        GL20C.glUseProgram(0)
        GL15C.glBindBuffer(GL15C.GL_ARRAY_BUFFER, 0)
        GL15C.glBindBuffer(GL15C.GL_ELEMENT_ARRAY_BUFFER, 0)
        GL30C.glBindVertexArray(0)
        GL11C.glDisable(GL11C.GL_BLEND)
        GL11C.glDisable(GL11C.GL_SCISSOR_TEST)
    }

    private fun destroy() {
        GL20C.glDetachShader(prog, vert_shdr)
        GL20C.glDetachShader(prog, frag_shdr)
        GL20C.glDeleteShader(vert_shdr)
        GL20C.glDeleteShader(frag_shdr)
        GL20C.glDeleteProgram(prog)
        GL11C.glDeleteTextures(default_font.texture().id())
        GL11C.glDeleteTextures(null_texture.texture().id())
        GL15C.glDeleteBuffers(vbo)
        GL15C.glDeleteBuffers(ebo)
        Nuklear.nk_buffer_free(cmds)
    }

    fun shutdown() {
        ctx.clip().copy()?.free()
        ctx.clip().paste()?.free()
        Nuklear.nk_free(ctx)
        destroy()
        default_font.query()?.free()
        default_font.width()?.free()

        ALLOCATOR.alloc()?.free()
        ALLOCATOR.mfree()?.free()
    }
}
