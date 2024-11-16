package dev.blackoutburst.game.world

import dev.blackoutburst.game.maths.Vector3i
import dev.blackoutburst.game.utils.main
import dev.blackoutburst.game.utils.stack
import org.lwjgl.opengl.GL30.*

class Chunk(
    val position: Vector3i,
    var blocks: ByteArray = ByteArray(4096)
) {
    var vaoID = 0
    var vboID = 0
    var eboID = 0

    private val internalXYZIndexObject = Vector3i()

    var vertices = mutableListOf<Int>()
    var indices = mutableListOf<Int>()
    var indexCount = 0

    fun clean() {
        if (vaoID != 0) glDeleteVertexArrays(vaoID)
        if (vboID != 0) glDeleteBuffers(vboID)
        if (eboID != 0) glDeleteBuffers(eboID)
    }

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

    init {
        World.chunkUpdate.incrementAndGet()

        genMesh()

        val vertexArray = vertices.toIntArray()
        val indexArray = indices.toIntArray()

        indices.clear()
        vertices.clear()

        main {
            vaoID = glGenVertexArrays()
            vboID = glGenBuffers()
            eboID = glGenBuffers()

            stack((vertexArray.size + indexArray.size) * 4) { stack ->
                glBindVertexArray(vaoID)

                // VAO
                glBindBuffer(GL_ARRAY_BUFFER, vboID)
                val vertexBuffer = stack.mallocInt(vertexArray.size)
                vertexBuffer.put(vertexArray).flip()
                glBufferData(GL_ARRAY_BUFFER, vertexBuffer, GL_STATIC_DRAW)
                glEnableVertexAttribArray(0)
                glVertexAttribIPointer(0, 1, GL_INT, 4, 0)

                //EBO
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID)
                val indexBuffer = stack.mallocInt(indexArray.size)
                indexBuffer.put(indexArray).flip()
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, GL_STATIC_DRAW)


                val oldChunk = World.chunks[position.toString()]

                World.chunks[position.toString()] = this

                oldChunk?.let {
                    glDeleteVertexArrays(it.vaoID)
                    glDeleteBuffers(it.vboID)
                    glDeleteBuffers(it.eboID)
                }

                World.chunkUpdate.decrementAndGet()
            }
        }
    }

    private fun genMesh() {
        var offset = 0
        block@for (i in 0 until 4096) {
            val block = blocks[i]
            if (block.toInt() == 0) continue@block
            val blockType = BlockType.getByID(block)

            _indexToXYZ(i)
            val x = internalXYZIndexObject.x
            val y = internalXYZIndexObject.y
            val z = internalXYZIndexObject.z

            val faces = getVisibleFaces(internalXYZIndexObject)

            face@for (j in 0 until 6) {
                if (!faces[j]) continue@face
                when (j) {
                    0 -> {

                        val t = blockType.textures[j]

                        vertices.add(packData(x, 1 + y, z, 0, 0, 0, t))
                        vertices.add(packData(1 + x, 1 + y, z, 1, 0, 0, t))
                        vertices.add(packData(1 + x, 1 + y, 1 + z, 1, 1, 0, t))
                        vertices.add(packData(x, 1 + y, 1 + z, 0, 1, 0, t))

                        indices.add(offset)
                        indices.add(2 + offset)
                        indices.add(1 + offset)
                        indices.add(offset)
                        indices.add(3 + offset)
                        indices.add(2 + offset)
                    }
                    1 -> {
                        val t = blockType.textures[j]

                        vertices.add(packData(x, y, z, 1, 1, 1, t))
                        vertices.add(packData(1 + x, y, z, 0, 1, 1, t))
                        vertices.add(packData(1 + x, 1 + y, z, 0, 0, 1, t))
                        vertices.add(packData(x, 1 + y, z, 1, 0, 1, t))

                        indices.add(offset)
                        indices.add(2 + offset)
                        indices.add(1 + offset)
                        indices.add(offset)
                        indices.add(3 + offset)
                        indices.add(2 + offset)
                    }
                    2 -> {
                        val t = blockType.textures[j]

                        vertices.add(packData(x, y, 1 + z, 0, 1, 2, t))
                        vertices.add(packData(1 + x, y, 1 + z, 1, 1, 2, t))
                        vertices.add(packData(1 + x, 1 + y, 1 + z, 1, 0, 2, t))
                        vertices.add(packData(x, 1 + y, 1 + z, 0, 0, 2, t))

                        indices.add(1 + offset)
                        indices.add(2 + offset)
                        indices.add(offset)
                        indices.add(2 + offset)
                        indices.add(3 + offset)
                        indices.add(offset)
                    }
                    3 -> {
                        val t = blockType.textures[j]

                        vertices.add(packData(x, y, z, 1, 1, 3, t))
                        vertices.add(packData(x, 1 + y, z, 1, 0, 3, t))
                        vertices.add(packData(x, 1 + y, 1 + z, 0, 0, 3, t))
                        vertices.add(packData(x, y, 1 + z, 0, 1, 3, t))

                        indices.add(offset)
                        indices.add(2 + offset)
                        indices.add(1 + offset)
                        indices.add(offset)
                        indices.add(3 + offset)
                        indices.add(2 + offset)
                    }
                    4 -> {
                        val t = blockType.textures[j]

                        vertices.add(packData(1 + x, y, z, 0, 1, 4, t))
                        vertices.add(packData(1 + x, 1 + y, z, 0, 0, 4, t))
                        vertices.add(packData(1 + x, 1 + y, 1 + z, 1, 0, 4, t))
                        vertices.add(packData(1 + x, y, 1 + z, 1, 1, 4, t))

                        indices.add(1 + offset)
                        indices.add(2 + offset)
                        indices.add(offset)
                        indices.add(2 + offset)
                        indices.add(3 + offset)
                        indices.add(offset)
                    }
                    5 -> {
                        val t = blockType.textures[j]

                        vertices.add(packData(x, y, z, 0, 1, 5, t))
                        vertices.add(packData(1 + x, y, z, 1, 1, 5, t))
                        vertices.add(packData(1 + x, y, 1 + z, 1, 0, 5, t))
                        vertices.add(packData(x, y, 1 + z, 0, 0, 5, t))

                        indices.add(1 + offset)
                        indices.add(2 + offset)
                        indices.add(offset)
                        indices.add(2 + offset)
                        indices.add(3 + offset)
                        indices.add(offset)
                    }
                }
                offset += 4
            }
        }
        indexCount = indices.size
    }

    fun render() {
        glBindVertexArray(vaoID)
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0)
    }

    fun indexToXYZ(index: Int): Vector3i = Vector3i(index % 16, (index / 16) % 16, (index / (16 * 16)) % 16)

    private fun _indexToXYZ(index: Int) = internalXYZIndexObject.set(index % 16, (index / 16) % 16, (index / (16 * 16)) % 16)

    fun xyzToIndex(x: Int, y: Int, z: Int): Int = x + 16 * (y + 16 * z)

    private fun getVisibleFaces(position: Vector3i): BooleanArray {
        val faces = BooleanArray(6)

        // TOP
        val b1 = getBlockAt(position.x, position.y + 1, position.z)
        faces[0] = (b1 == null || b1 == BlockType.AIR || b1.transparent)

        // FRONT
        val b2 = getBlockAt(position.x, position.y, position.z - 1)
        faces[1] = (b2 == null || b2 == BlockType.AIR || b2.transparent)

        // BACK
        val b3 = getBlockAt(position.x, position.y, position.z + 1)
        faces[2] = (b3 == null || b3 == BlockType.AIR || b3.transparent)

        // LEFT
        val b4 = getBlockAt(position.x - 1, position.y, position.z)
        faces[3] = (b4 == null || b4 == BlockType.AIR || b4.transparent)

        // RIGHT
        val b5 = getBlockAt(position.x + 1, position.y, position.z)
        faces[4] = (b5 == null || b5 == BlockType.AIR || b5.transparent)

        // BOTTOM
        val b6 = getBlockAt(position.x, position.y - 1, position.z)
        faces[5] = (b6 == null || b6 == BlockType.AIR || b6.transparent)

        return faces
    }

    private fun getBlockAt(x: Int, y: Int, z: Int): BlockType? {
        if (x < 0 || x > 15 || y < 0 || y > 15 || z < 0 || z > 15) return null

        return try {
            val blockId = xyzToIndex(x, y, z)

            return BlockType.getByID(blocks[blockId])
        } catch (ignored: Exception) {
            null
        }
    }

    companion object {
        fun getIndex(position: Vector3i): Vector3i {
            return Vector3i(
                (if (position.x < 0) (position.x + 1) / CHUNK_SIZE - 1 else position.x / CHUNK_SIZE) * CHUNK_SIZE,
                (if (position.y < 0) (position.y + 1) / CHUNK_SIZE - 1 else position.y / CHUNK_SIZE) * CHUNK_SIZE,
                (if (position.z < 0) (position.z + 1) / CHUNK_SIZE - 1 else position.z / CHUNK_SIZE) * CHUNK_SIZE
            )
        }
    }
}
