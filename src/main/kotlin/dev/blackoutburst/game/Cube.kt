package dev.blackoutburst.game

import dev.blackoutburst.game.camera.Camera
import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.shader.Shader
import dev.blackoutburst.game.shader.ShaderProgram
import dev.blackoutburst.game.texture.TextureArray
import dev.blackoutburst.game.utils.Color
import dev.blackoutburst.game.utils.Textures
import dev.blackoutburst.game.utils.main
import dev.blackoutburst.game.utils.stack
import dev.blackoutburst.game.world.BlockType
import org.lwjgl.opengl.GL30.*

class Cube {
    var vaoID = 0
    var vboID = 0
    var eboID = 0

    private val diffuseMap = TextureArray(listOf(
        "steve/top.png",
        "steve/front.png",
        "steve/back.png",
        "steve/left.png",
        "steve/right.png",
        "steve/bottom.png",
    ), 8).id

    private val vertexShader = Shader(GL_VERTEX_SHADER, "/shaders/cube.vert")
    private val fragmentShader = Shader(GL_FRAGMENT_SHADER, "/shaders/cube.frag")
    private val shaderProgram = ShaderProgram(vertexShader, fragmentShader)

    val vertices = intArrayOf(
        //TOP
        packData(0, 1, 0, 0, 0, 0, 0),
        packData(1, 1, 0, 1, 0, 0, 0),
        packData(1, 1, 1, 1, 1, 0, 0),
        packData(0, 1, 1, 0, 1, 0, 0),

        //FRONT
        packData(0, 0, 0, 0, 0, 1, 1),
        packData(1, 0, 0, 1, 0, 1, 1),
        packData(1, 1, 0, 1, 1, 1, 1),
        packData(0, 1, 0, 0, 1, 1, 1),

        //BACK
        packData(0, 0, 1, 1, 0, 2, 2),
        packData(1, 0, 1, 0, 0, 2, 2),
        packData(1, 1, 1, 0, 1, 2, 2),
        packData(0, 1, 1, 1, 1, 2, 2),

        //LEFT
        packData(0, 0, 0, 0, 0, 3, 3),
        packData(0, 1, 0, 0, 1, 3, 3),
        packData(0, 1, 1, 1, 1, 3, 3),
        packData(0, 0, 1, 1, 0, 3, 3),

        //RIGHT
        packData(1, 0, 0, 1, 0, 4, 4),
        packData(1, 1, 0, 1, 1, 4, 4),
        packData(1, 1, 1, 0, 1, 4, 4),
        packData(1, 0, 1, 0, 0, 4, 4),

        //BOTTOM
        packData(0, 0, 0, 1, 0, 5, 5),
        packData(1, 0, 0, 0, 0, 5, 5),
        packData(1, 0, 1, 0, 1, 5, 5),
        packData(0, 0, 1, 1, 1, 5, 5),
    )

    val indices = intArrayOf(
        //TOP
        0, 2, 1,
        0, 3, 2,

        //FRONT
        4, 6, 5,
        4, 7, 6,

        //BACK
        9, 10, 8,
        10, 11, 8,

        //LEFT
        12, 14, 13,
        12, 15, 14,

        //RIGHT
        17, 18, 16,
        18, 19, 16,

        //BOTTOM
        21, 22, 20,
        22, 23, 20,
    )

    private fun packData(
        x: Int,
        y: Int,
        z: Int,
        u: Int,
        v: Int,
        f: Int,
        t: Int,
    ): Int = (x and 31) or
            ((y and 31) shl 5) or
            ((z and 31) shl 10) or
            ((u and 31) shl 15) or
            ((v and 31) shl 20) or
            ((f and 7) shl 25) or
            ((t and 31) shl 28)

    init {
        main {
            if (vaoID == 0) {
                vaoID = glGenVertexArrays()
                vboID = glGenBuffers()
                eboID = glGenBuffers()
            }

            stack { stack ->
                glBindVertexArray(vaoID)

                // VAO
                glBindBuffer(GL_ARRAY_BUFFER, vboID)
                val vertexBuffer = stack.mallocInt(vertices.size)
                vertexBuffer.put(vertices).flip()
                glBufferData(GL_ARRAY_BUFFER, vertexBuffer, GL_STATIC_DRAW)
                glEnableVertexAttribArray(0)
                glVertexAttribIPointer(0, 1, GL_INT, 4, 0)
                glBindBuffer(GL_ARRAY_BUFFER, 0)

                //EBO
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID)
                val indexBuffer = stack.mallocInt(indices.size)
                indexBuffer.put(indices).flip()
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, GL_STATIC_DRAW)

                glBindVertexArray(0)
            }
        }
    }

    fun render(transformation: Matrix) {
        glUseProgram(shaderProgram.id)
        shaderProgram.setUniform1i("diffuseMap", 0)
        shaderProgram.setUniformMat4("model", transformation)
        shaderProgram.setUniformMat4("view", Camera.view)
        shaderProgram.setUniformMat4("projection", Matrix().projectionMatrix(90f, 1000f, 0.1f))
        shaderProgram.setUniform4f("color", Color.WHITE)
        shaderProgram.setUniform3f("lightColor", Color.WHITE)
        shaderProgram.setUniform3f("viewPos", Camera.position)


        glActiveTexture(GL_TEXTURE0)
        glBindTexture(GL_TEXTURE_2D_ARRAY, diffuseMap)

        glBindVertexArray(vaoID)
        glDrawElements(GL_TRIANGLES, indices.size, GL_UNSIGNED_INT, 0)
    }
}