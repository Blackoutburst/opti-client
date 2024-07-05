package dev.blackoutburst.game.world

import dev.blackoutburst.game.maths.Vector3i
import dev.blackoutburst.game.utils.main
import dev.blackoutburst.game.utils.stack
import org.lwjgl.opengl.GL30.*

class Chunk(
    val position: Vector3i,
    var blocks: Array<Byte> = Array(4096) { BlockType.AIR.id }
) {
    var vaoID = 0
    var vboID = 0
    var eboID = 0

    companion object {
        val vertices = intArrayOf(
            packData(0, 1, 0, 0, 0, 0, 1),
            packData(1, 1, 0, 1, 0, 0, 1),
            packData(1, 1, 1, 1, 1, 0, 1),
            packData(0, 1, 1, 0, 1, 0, 1),
        )

        val indices = intArrayOf(
            0, 2, 1,
            0, 3, 2,
        )

        private fun packData(
            x: Int,
            y: Int,
            z: Int,
            u: Int,
            v: Int,
            n: Int,
            t: Int,
        ): Int = (x and 31) or
                ((y and 31) shl 5) or
                ((z and 31) shl 10) or
                ((u and 31) shl 15) or
                ((v and 31) shl 20) or
                ((n and 7) shl 25) or
                ((t and 15) shl 28)
    }

    fun update() {
        main {
            if (vaoID == 0) {
                vaoID = glGenVertexArrays()
                vboID = glGenBuffers()
                eboID = glGenBuffers()
            }

            glBindVertexArray(vaoID)

            stack { stack ->
                glBindBuffer(GL_ARRAY_BUFFER, vboID)

                val vertexBuffer = stack.mallocInt(vertices.size)
                vertexBuffer.put(vertices).flip()

                glBufferData(GL_ARRAY_BUFFER, vertexBuffer, GL_STATIC_DRAW)

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID)

                val indexBuffer = stack.mallocInt(indices.size)
                indexBuffer.put(indices).flip()

                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, GL_STATIC_DRAW)

                glEnableVertexAttribArray(0)
                glVertexAttribIPointer(0, 1, GL_INT, 4, 0)
                glBindVertexArray(0)
            }
        }
    }

    fun render() {
        glBindVertexArray(vaoID)
        glDrawElements(GL_TRIANGLES, indices.size, GL_UNSIGNED_INT, 0)
        glBindVertexArray(0)
    }
}