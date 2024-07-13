package dev.blackoutburst.game.ui

import dev.blackoutburst.game.Main
import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.shader.Shader
import dev.blackoutburst.game.shader.ShaderProgram
import dev.blackoutburst.game.texture.Texture
import dev.blackoutburst.game.texture.TextureArray
import dev.blackoutburst.game.utils.Textures
import dev.blackoutburst.game.utils.stack
import dev.blackoutburst.game.window.Window
import dev.blackoutburst.game.world.BlockType
import org.lwjgl.opengl.GL30.*

object Cursor {

    private val blockTextures = TextureArray(Textures.entries.map { it.file }).id

    private val vertices = floatArrayOf(
        0f, 0f, 0f, 0f,
        1f, 1f, 1f, 1f,
        0f, 1f, 0f, 1f,
        1f, 0f, 1f, 0f,
    )

    private val indices = intArrayOf(
        0, 1, 2,
        0, 3, 1,
    )

    private var vaoID = 0

    private val texture = Texture("./cursor.png")
    private val vertexShader = Shader(GL_VERTEX_SHADER, "/shaders/2D.vert")
    private val fragmentShader = Shader(GL_FRAGMENT_SHADER, "/shaders/AWP.frag")
    private val shaderProgram = ShaderProgram(vertexShader, fragmentShader)

    init {
        vaoID = glGenVertexArrays()
        val vboID = glGenBuffers()
        val eboID = glGenBuffers()

        stack { stack ->
            glBindVertexArray(vaoID)

            glBindBuffer(GL_ARRAY_BUFFER, vboID)
            val vertexBuffer = stack.mallocFloat(vertices.size)
            vertexBuffer.put(vertices).flip()
            glBufferData(GL_ARRAY_BUFFER, vertexBuffer, GL_STATIC_DRAW)

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID)
            val indexBuffer = stack.mallocInt(indices.size)
            indexBuffer.put(indices).flip()
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, GL_STATIC_DRAW)

            glEnableVertexAttribArray(0)
            glVertexAttribPointer(0, 2, GL_FLOAT, false, 16, 0)
            glEnableVertexAttribArray(1)
            glVertexAttribPointer(1, 2, GL_FLOAT, false, 16, 8)
            glBindVertexArray(0)
        }
    }

    fun render() {
        val layer = if (Main.blockType == BlockType.GRASS) Main.blockType.textures[0].toFloat() else Main.blockType.textures[1].toFloat()

        glUseProgram(shaderProgram.id)
        shaderProgram.setUniform1i("text", 0)
        shaderProgram.setUniform1i("diffuseMap", 1)
        shaderProgram.setUniform1f("layer", layer)
        shaderProgram.setUniformMat4("model", Matrix().scale(Vector2f(Window.width.toFloat(), Window.height.toFloat())))
        shaderProgram.setUniformMat4("projection", Matrix().ortho2D(0f, Window.width.toFloat(), 0f, Window.height.toFloat(), -1f, 1f))

        glActiveTexture(GL_TEXTURE0)
        glBindTexture(GL_TEXTURE_2D_ARRAY, blockTextures)

        glActiveTexture(GL_TEXTURE1)
        glBindTexture(GL_TEXTURE_2D, texture.id)

        glBindVertexArray(vaoID)
        glDrawElements(GL_TRIANGLES, indices.size, GL_UNSIGNED_INT, 0)
    }
}