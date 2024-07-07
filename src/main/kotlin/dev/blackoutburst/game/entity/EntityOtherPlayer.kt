package dev.blackoutburst.game.entity

import dev.blackoutburst.game.camera.Camera
import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.shader.Shader
import dev.blackoutburst.game.shader.ShaderProgram
import dev.blackoutburst.game.texture.Texture
import dev.blackoutburst.game.utils.Color
import dev.blackoutburst.game.utils.Time
import org.lwjgl.opengl.GL30.*
import kotlin.math.abs
import kotlin.math.cos
import kotlin.math.pow
import kotlin.math.sin

class EntityOtherPlayer(
    id: Int,
    position: Vector3f = Vector3f(),
    rotation: Vector2f = Vector2f(),
) : Entity(id, position, rotation) {

    private val texture = Texture("steve.png", false)
    private val vertexShader = Shader(GL_VERTEX_SHADER, "/shaders/cube.vert")
    private val fragmentShader = Shader(GL_FRAGMENT_SHADER, "/shaders/cube.frag")
    private val shaderProgram = ShaderProgram(vertexShader, fragmentShader)

    private var bodyRotation = 0f

    private var swingRotation = 0f
    private var invertRotation = false

    private val SWING_SPEED = 300f

    private var previousPosition = position

    override fun update() {
    }

    override fun render() {
        val moving = (previousPosition.x - position.x).pow(2) + (previousPosition.z - position.z).pow(2) > 0
        previousPosition = position

        val xRad = -rotation.x
        val yRad = -rotation.y

        if (rotation.x - bodyRotation > if (moving) 0.1f else 0.75f) {
            bodyRotation += 0.01f
        } else if (rotation.x - bodyRotation < if (moving) -0.1f else -0.75f) {
            bodyRotation -= 0.01f
        }

        val bodyRad = -bodyRotation
        if (moving) {
            if (swingRotation > 60f && !invertRotation)
                invertRotation = true
            if (swingRotation < -60f && invertRotation)
                invertRotation = false

            if (invertRotation)
                swingRotation -= SWING_SPEED * Time.delta.toFloat()
            else
                swingRotation += SWING_SPEED * Time.delta.toFloat()
        } else {
            if (swingRotation > 0)
                swingRotation -= SWING_SPEED * Time.delta.toFloat()
            if (swingRotation < 0)
                swingRotation += SWING_SPEED * Time.delta.toFloat()
        }

        val radSwing = Math.toRadians(swingRotation.toDouble()).toFloat()
        val offset = Vector3f(-0.5f)

        glUseProgram(shaderProgram.id)
        shaderProgram.setUniform1i("diffuseMap", 0)
        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .translate(Vector3f(0.5f, 0.0f, 0.5f))
                .rotate(yRad, Vector3f(cos(xRad), 0f, -sin(xRad)))
                .rotate(xRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 0.0f, -0.5f))
        )
        shaderProgram.setUniformMat4("view", Camera.view)
        shaderProgram.setUniformMat4("projection", Matrix().projectionMatrix(90f, 1000f, 0.1f))
        shaderProgram.setUniform4f("color", Color.WHITE)
        shaderProgram.setUniform3f("lightColor", Color.WHITE)
        shaderProgram.setUniform3f("viewPos", Camera.position)


        glActiveTexture(GL_TEXTURE0)
        glBindTexture(GL_TEXTURE_2D, texture.id)

        glBindVertexArray(PlayerModelMale.vaos["head"]!!)
        glDrawElements(GL_TRIANGLES, PlayerModelMale.indices.size, GL_UNSIGNED_INT, 0)

        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .translate(Vector3f(0.5f))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f))
        )

        glBindVertexArray(PlayerModelMale.vaos["body"]!!)
        glDrawElements(GL_TRIANGLES, PlayerModelMale.indices.size, GL_UNSIGNED_INT, 0)


        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .translate(Vector3f(0.5f, 0.0f, 0.5f))
                .rotate(radSwing, Vector3f(cos(bodyRad), 0f, -sin(bodyRad)))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 0.0f, -0.5f))
        )

        glBindVertexArray(PlayerModelMale.vaos["leftArm"]!!)
        glDrawElements(GL_TRIANGLES, PlayerModelMale.indices.size, GL_UNSIGNED_INT, 0)

        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .translate(Vector3f(0.5f, 0.0f, 0.5f))
                .rotate(-radSwing, Vector3f(cos(bodyRad), 0f, -sin(bodyRad)))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 0.0f, -0.5f))
        )

        glBindVertexArray(PlayerModelMale.vaos["rightArm"]!!)
        glDrawElements(GL_TRIANGLES, PlayerModelMale.indices.size, GL_UNSIGNED_INT, 0)


        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .translate(Vector3f(0.5f, -1.5f, 0.5f))
                .rotate(-radSwing, Vector3f(cos(bodyRad), 0f, -sin(bodyRad)))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 1.5f, -0.5f))
        )

        glBindVertexArray(PlayerModelMale.vaos["leftLeg"]!!)
        glDrawElements(GL_TRIANGLES, PlayerModelMale.indices.size, GL_UNSIGNED_INT, 0)

        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .translate(Vector3f(0.5f, -1.5f, 0.5f))
                .rotate(radSwing, Vector3f(cos(bodyRad), 0f, -sin(bodyRad)))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 1.5f, -0.5f))
        )

        glBindVertexArray(PlayerModelMale.vaos["rightLeg"]!!)
        glDrawElements(GL_TRIANGLES, PlayerModelMale.indices.size, GL_UNSIGNED_INT, 0)
    }
}