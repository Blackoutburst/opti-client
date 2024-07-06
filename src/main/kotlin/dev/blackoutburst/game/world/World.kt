package dev.blackoutburst.game.world

import dev.blackoutburst.game.camera.Camera
import dev.blackoutburst.game.camera.FrustumCulling
import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.maths.Vector3i
import dev.blackoutburst.game.shader.Shader
import dev.blackoutburst.game.shader.ShaderProgram
import dev.blackoutburst.game.texture.TextureArray
import dev.blackoutburst.game.utils.Color
import dev.blackoutburst.game.utils.RayCastResult
import dev.blackoutburst.game.utils.Textures
import org.lwjgl.opengl.GL20.GL_FRAGMENT_SHADER
import org.lwjgl.opengl.GL20.GL_VERTEX_SHADER
import org.lwjgl.opengl.GL30.*
import java.util.concurrent.ConcurrentHashMap
import java.util.concurrent.atomic.AtomicInteger
import kotlin.math.abs
import kotlin.math.floor
import kotlin.math.sign

const val CHUNK_SIZE = 16

object World {
    private val vertexShader = Shader(GL_VERTEX_SHADER, "/shaders/chunk.vert")
    private val fragmentShader = Shader(GL_FRAGMENT_SHADER, "/shaders/chunk.frag")
    private val shaderProgram = ShaderProgram(vertexShader, fragmentShader)

    private val diffuseMap = TextureArray(Textures.entries.map { it.file }).id

    val chunks: ConcurrentHashMap<String, Chunk> = ConcurrentHashMap()
    var chunkRendered = 0
    var chunkUpdate = AtomicInteger(0)

    fun addChunk(position: Vector3i, blockData: Array<Byte>) {
        chunks[position.toString()]?.let {
            it.blocks = blockData
            it.update()
        } ?: run {
            val chunk = Chunk(position, blockData)
            chunk.update()

            chunks[position.toString()] = chunk
        }
    }

    private fun setUniforms() {
        shaderProgram.setUniform4f("color", Color.WHITE)
        shaderProgram.setUniform3f("lightColor", Color.WHITE)
        shaderProgram.setUniform3f("viewPos", Camera.position)

        shaderProgram.setUniformMat4("projection", Matrix().projectionMatrix(90f, 1000f, 0.1f))
        shaderProgram.setUniformMat4("view", Camera.view)
    }

    fun render() {
        glBindTexture(GL_TEXTURE_2D_ARRAY, diffuseMap)

        glUseProgram(shaderProgram.id)

        setUniforms()

        glDisable(GL_CULL_FACE)

        FrustumCulling.update(Matrix().projectionMatrix(90f, 1000f, 0.1f), Camera.view)

        chunkRendered = 0
        for (chunk in chunks.values) {
            if (chunk.instanceCount == 0 || !FrustumCulling.isInFrustum(chunk)) continue

            shaderProgram.setUniform3f("chunkPos", chunk.position.toFloat())
            chunk.render()
            chunkRendered++
        }
        glEnable(GL_CULL_FACE)
    }

    fun getBlockAt(position: Vector3i): Block? {
        val chunkPosition = Chunk.getIndex(position, CHUNK_SIZE)
        val chunk = chunks[chunkPosition.toString()] ?: return null
        val positionAsInt = Vector3i(
            ((position.x - chunk.position.x) % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE,
            ((position.y - chunk.position.y) % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE,
            ((position.z - chunk.position.z) % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE
        )

        val blockId = chunk.xyzToIndex(positionAsInt.x, positionAsInt.y, positionAsInt.z)
        val blockType = chunk.blocks[blockId]
        if (blockType == BlockType.AIR.id) return null

        return Block(BlockType.getByID(chunk.blocks[blockId]), chunk.indexToXYZ(blockId) + chunk.position)
    }

    fun dda(rayPos: Vector3f, rayDir: Vector3f, maxRaySteps: Int): RayCastResult {
        val mapPos = Vector3i(floor(rayPos.x).toInt(), floor(rayPos.y).toInt(), floor(rayPos.z).toInt())
        val rayDirLength = rayDir.length()
        val deltaDist = Vector3f(abs(rayDirLength / rayDir.x), abs(rayDirLength / rayDir.y), abs(rayDirLength / rayDir.z))
        val rayStep = Vector3i(sign(rayDir.x).toInt(), sign(rayDir.y).toInt(), sign(rayDir.z).toInt())
        val signRayDir = Vector3f(sign(rayDir.x), sign(rayDir.y), sign(rayDir.z))
        val mapPosVec3 = Vector3f(mapPos.x.toFloat(), mapPos.y.toFloat(), mapPos.z.toFloat())
        val sideDist = (signRayDir * (mapPosVec3 - rayPos) + (signRayDir * 0.5f) + 0.5f) * deltaDist
        var mask = Vector3i()

        for (i in 0 until maxRaySteps) {
            val block = getBlockAt(mapPos)
            if (block != null) return RayCastResult(block, mask)

            if (sideDist.x < sideDist.y) {
                if (sideDist.x < sideDist.z) {
                    sideDist.x += deltaDist.x
                    mapPos.x += rayStep.x
                    mask = Vector3i(-rayStep.x, 0, 0)
                } else {
                    sideDist.z += deltaDist.z
                    mapPos.z += rayStep.z
                    mask = Vector3i(0, 0, -rayStep.z)
                }
            } else {
                if (sideDist.y < sideDist.z) {
                    sideDist.y += deltaDist.y
                    mapPos.y += rayStep.y
                    mask = Vector3i(0, -rayStep.y, 0)
                } else {
                    sideDist.z += deltaDist.z
                    mapPos.z += rayStep.z
                    mask = Vector3i(0, 0, -rayStep.z)
                }
            }
        }

        return RayCastResult(null, mask)
    }
}