package dev.blackoutburst.game.world

import dev.blackoutburst.game.maths.Vector3i
import dev.blackoutburst.game.utils.main
import dev.blackoutburst.game.utils.stack
import org.lwjgl.opengl.ARBInstancedArrays
import org.lwjgl.opengl.GL30.*
import org.lwjgl.opengl.GL33.glDrawElementsInstanced
import org.lwjgl.opengl.GL33.glVertexAttribDivisor
import org.lwjgl.system.MemoryStack
import java.nio.IntBuffer
import kotlin.random.Random

class Chunk(
    val position: Vector3i,
    var blocks: Array<Byte> = Array(4096) { BlockType.AIR.id }
) {
    var vaoID = 0
    var vboID = 0
    var eboID = 0
    var ssboId = 0
    var instanceCount = 0

    val vertices = intArrayOf(
        packData(0, 1, 0, 0, 0),
        packData(1, 1, 0, 1, 0),
        packData(1, 1, 1, 1, 1),
        packData(0, 1, 1, 0, 1),
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
    ): Int = (x and 31) or
            ((y and 31) shl 5) or
            ((z and 31) shl 10) or
            ((u and 31) shl 15) or
            ((v and 31) shl 20)

    private fun packData(
        position: Vector3i,
        face: Int,
        texture: Int
    ): Int = (position.x and 31) or
            ((position.y and 31) shl 5) or
            ((position.z and 31) shl 10) or
            ((face and 7) shl 15) or
            ((texture and 31) shl 18)

    fun update() {
        main {
            if (vaoID == 0) {
                vaoID = glGenVertexArrays()
                vboID = glGenBuffers()
                eboID = glGenBuffers()
                ssboId = glGenBuffers()
            }

            stack(128 * 1024) { stack ->
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

                //SSBO
                glBindBuffer(GL_ARRAY_BUFFER, ssboId)
                val instanceBuffer = getOffsets(stack)
                instanceBuffer.flip()
                glBufferData(GL_ARRAY_BUFFER, instanceBuffer, GL_STATIC_DRAW)
                glEnableVertexAttribArray(1)
                glVertexAttribIPointer(1, 1, GL_INT, 0, 0)
                ARBInstancedArrays.glVertexAttribDivisorARB(1, 1)
                glBindBuffer(GL_ARRAY_BUFFER, 0)

                glBindVertexArray(0)
            }
        }
    }

    private fun getOffsets(stack: MemoryStack): IntBuffer {
        val buffer = stack.mallocInt(4096 * 6)
        instanceCount = 0

        block@for (i in blocks.indices) {
            val block = blocks[i]
            if (block.toInt() == 0) continue@block
            val blockType = BlockType.getByID(block)

            val position = indexToXYZ(i)
            val faces = getVisibleFaces(position)

            face@for (j in faces.indices) {
                val face = faces[j]
                if (!face) continue@face

                buffer.put(packData(position, j, blockType.textures[j]))
                instanceCount++
            }
        }

        return buffer
    }

    fun render() {
        glBindVertexArray(vaoID)
        glDrawElementsInstanced(GL_TRIANGLES, indices.size, GL_UNSIGNED_INT, 0, instanceCount)
        glBindVertexArray(0)
    }

    fun indexToXYZ(index: Int): Vector3i = Vector3i(index % 16, (index / 16) % 16, (index / (16 * 16)) % 16)

    fun xyzToIndex(x: Int, y: Int, z: Int): Int = x + 16 * (y + 16 * z)

    private fun getVisibleFaces(position: Vector3i): Array<Boolean> {
        val faces = Array(6) { false }
        val chunkX = this@Chunk.position.x
        val chunkY = this@Chunk.position.y
        val chunkZ = this@Chunk.position.z

        // TOP
        run {
            val b = World.getBlockAt(Vector3i(chunkX + position.x, chunkY + position.y + 1, chunkZ + position.z))
            if (b == null || b.type == BlockType.AIR || b.type.transparent) {
                faces[0] = true
            }
        }

        // FRONT
        run {
            val b = World.getBlockAt(Vector3i(chunkX + position.x, chunkY + position.y, chunkZ + position.z - 1))
            if (b == null || b.type == BlockType.AIR || b.type.transparent) {
                faces[1] = true
            }
        }

        // BACK
        run {
            val b = World.getBlockAt(Vector3i(chunkX + position.x, chunkY + position.y, chunkZ + position.z + 1))
            if (b == null || b.type == BlockType.AIR || b.type.transparent) {
                faces[2] = true
            }
        }

        // LEFT
        run {
            val b = World.getBlockAt(Vector3i(chunkX + position.x - 1, chunkY + position.y, chunkZ + position.z))
            if (b == null || b.type == BlockType.AIR || b.type.transparent) {
                faces[3] = true
            }
        }

        // RIGHT
        run {
            val b = World.getBlockAt(Vector3i(chunkX + position.x + 1, chunkY + position.y, chunkZ + position.z))
            if (b == null || b.type == BlockType.AIR || b.type.transparent) {
                faces[4] = true
            }
        }

        // BOTTOM
        run {
            val b = World.getBlockAt(Vector3i(chunkX + position.x, chunkY + position.y - 1, chunkZ + position.z))
            if (b == null || b.type == BlockType.AIR || b.type.transparent) {
                faces[5] = true
            }
        }

        return faces
    }

    companion object {
        fun getIndex(position: Vector3i, chunkSize: Int): Vector3i {
            return Vector3i(
                (if (position.x < 0) (position.x + 1) / chunkSize - 1 else position.x / chunkSize) * chunkSize,
                (if (position.y < 0) (position.y + 1) / chunkSize - 1 else position.y / chunkSize) * chunkSize,
                (if (position.z < 0) (position.z + 1) / chunkSize - 1 else position.z / chunkSize) * chunkSize
            )
        }
    }
}
