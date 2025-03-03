package dev.blackoutburst.game.entity

import dev.blackoutburst.game.camera.Camera
import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.shader.Shader
import dev.blackoutburst.game.shader.ShaderProgram
import dev.blackoutburst.game.texture.Texture
import dev.blackoutburst.game.utils.Color
import dev.blackoutburst.game.utils.SkinAPI
import dev.blackoutburst.game.utils.Time
import dev.blackoutburst.game.utils.expDecay
import org.lwjgl.opengl.GL30.*
import kotlin.math.cos
import kotlin.math.pow
import kotlin.math.sin

class EntityOtherPlayer(
    id: Int,
    position: Vector3f = Vector3f(),
    rotation: Vector2f = Vector2f(),
    name: String,
) : Entity(id, position, rotation, name) {
    private val skin = SkinAPI.skins.find { it.name == name }
    private val texture = if (skin != null) Texture(skin.data, false) else Texture("pol.png", false)
    private val model = if (skin?.model == "slim") PlayerModelFemale(texture.id) else PlayerModelMale(texture.id)
    private val vertexShader = Shader(GL_VERTEX_SHADER, "/shaders/cube.vert")
    private val fragmentShader = Shader(GL_FRAGMENT_SHADER, "/shaders/cube.frag")
    private val shaderProgram = ShaderProgram(vertexShader, fragmentShader)

    private var bodyRotation = 0f

    private var swingRotation = 0f

    private var moving = false

    override fun update() {
        moving = (previousRawPosition.x - rawPosition.x).pow(2) + (previousRawPosition.z - rawPosition.z).pow(2) != 0f
    }

    override fun render(view: Matrix, projection: Matrix) {
        val xRad = -rotation.x
        val yRad = -rotation.y

        if (rotation.x - bodyRotation > if (moving) 0.1f else 0.75f) {
            bodyRotation += 10f * Time.delta.toFloat()
        } else if (rotation.x - bodyRotation < if (moving) -0.1f else -0.75f) {
            bodyRotation -= 10f * Time.delta.toFloat()
        }

        val bodyRad = -bodyRotation
        if (moving) {
            swingRotation += 4f * Time.delta.toFloat()
        } else {
            swingRotation = if (swingRotation > Math.PI)
                expDecay(swingRotation, Math.PI.toFloat() * 2f, 5f, Time.delta)
            else
                expDecay(swingRotation, 0f, 5f, Time.delta)
        }
        if (swingRotation > Math.PI * 2) {
            swingRotation = 0f
        }

        val radSwing = sin(swingRotation) * 0.85f
        val offset = Vector3f(-0.25f, -0.25f, -0.25f)

        shaderProgram.setUniformMat4("view", view)
        shaderProgram.setUniformMat4("projection", projection)
        shaderProgram.setUniform4f("color", Color.WHITE)
        shaderProgram.setUniform3f("lightColor", Color.WHITE)
        shaderProgram.setUniform3f("viewPos", Camera.position)

        glUseProgram(shaderProgram.id)
        shaderProgram.setUniform1i("diffuseMap", 0)
        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .scale(Vector3f(0.5f))
                .translate(Vector3f(0.5f, 0.0f, 0.5f))
                .rotate(yRad, Vector3f(cos(xRad), 0f, -sin(xRad)))
                .rotate(xRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 0.0f, -0.5f))

        )

        glActiveTexture(GL_TEXTURE0)
        glBindTexture(GL_TEXTURE_2D, texture.id)

        glBindVertexArray(model.vaos["head"]!!)
        glDrawElements(GL_TRIANGLES, model.indices["head"]!!.size, GL_UNSIGNED_INT, 0)

        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .scale(Vector3f(0.5f))
                .translate(Vector3f(0.5f))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f))

        )

        glBindVertexArray(model.vaos["body"]!!)
        glDrawElements(GL_TRIANGLES, model.indices["body"]!!.size, GL_UNSIGNED_INT, 0)


        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .scale(Vector3f(0.5f))
                .translate(Vector3f(0.5f, 0.0f, 0.5f))
                .rotate(radSwing, Vector3f(cos(bodyRad), 0f, -sin(bodyRad)))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 0.0f, -0.5f))
        )

        glBindVertexArray(model.vaos["leftArm"]!!)
        glDrawElements(GL_TRIANGLES, model.indices["leftArm"]!!.size, GL_UNSIGNED_INT, 0)

        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .scale(Vector3f(0.5f))
                .translate(Vector3f(0.5f, 0.0f, 0.5f))
                .rotate(-radSwing, Vector3f(cos(bodyRad), 0f, -sin(bodyRad)))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 0.0f, -0.5f))
        )

        glBindVertexArray(model.vaos["rightArm"]!!)
        glDrawElements(GL_TRIANGLES, model.indices["rightArm"]!!.size, GL_UNSIGNED_INT, 0)


        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .scale(Vector3f(0.5f))
                .translate(Vector3f(0.5f, -1.5f, 0.5f))
                .rotate(-radSwing, Vector3f(cos(bodyRad), 0f, -sin(bodyRad)))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 1.5f, -0.5f))
        )

        glBindVertexArray(model.vaos["leftLeg"]!!)
        glDrawElements(GL_TRIANGLES, model.indices["leftLeg"]!!.size, GL_UNSIGNED_INT, 0)

        shaderProgram.setUniformMat4("model",
            Matrix()
                .translate(position)
                .translate(offset)
                .scale(Vector3f(0.5f))
                .translate(Vector3f(0.5f, -1.5f, 0.5f))
                .rotate(radSwing, Vector3f(cos(bodyRad), 0f, -sin(bodyRad)))
                .rotate(bodyRad, Vector3f(0f, 1f, 0f))
                .translate(Vector3f(-0.5f, 1.5f, -0.5f))
        )

        glBindVertexArray(model.vaos["rightLeg"]!!)
        glDrawElements(GL_TRIANGLES, model.indices["rightLeg"]!!.size, GL_UNSIGNED_INT, 0)
    }
}