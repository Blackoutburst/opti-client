package dev.blackoutburst.game.world

import dev.blackoutburst.game.Camera
import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector3i
import dev.blackoutburst.game.shader.Shader
import dev.blackoutburst.game.shader.ShaderProgram
import dev.blackoutburst.game.texture.TextureArray
import dev.blackoutburst.game.utils.Color
import dev.blackoutburst.game.utils.Textures
import dev.blackoutburst.game.utils.stack
import org.lwjgl.opengl.GL14
import org.lwjgl.opengl.GL20.GL_FRAGMENT_SHADER
import org.lwjgl.opengl.GL20.GL_VERTEX_SHADER
import org.lwjgl.opengl.GL30.*
import java.util.concurrent.ConcurrentHashMap

const val CHUNK_SIZE = 16

object World {
    private val vertexShader = Shader(GL_VERTEX_SHADER, "/shaders/chunk.vert")
    private val fragmentShader = Shader(GL_FRAGMENT_SHADER, "/shaders/chunk.frag")
    private val shaderProgram = ShaderProgram(vertexShader, fragmentShader)

    private val diffuseMap = TextureArray(Textures.entries.map { it.file }).id

    val chunks: ConcurrentHashMap<String, Chunk> = ConcurrentHashMap()

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

        shaderProgram.setUniformMat4("projection", Matrix().projectionMatrix(90f, 0.1f, 1000f))
        shaderProgram.setUniformMat4("view", Camera.view)
    }

    fun render() {
        glBindTexture(GL_TEXTURE_2D_ARRAY, diffuseMap)

        glUseProgram(shaderProgram.id)

        setUniforms()

        for (chunk in chunks.values) {
            shaderProgram.setUniform3f("chunkPos", chunk.position.toFloat())
            chunk.render()
        }
    }
}