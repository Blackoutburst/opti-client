package dev.blackoutburst.game.entity

import dev.blackoutburst.game.Camera
import dev.blackoutburst.game.input.Keyboard
import dev.blackoutburst.game.input.Mouse
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.utils.Time
import org.lwjgl.glfw.GLFW
import kotlin.math.cos
import kotlin.math.sin

class EntityPlayer(
    id: Int,
    position: Vector3f = Vector3f(),
    rotation: Vector2f = Vector2f(),
): Entity(id, position, rotation) {

    private var lastMousePosition = Mouse.position
    private val sensitivity = 0.1f

    private var moving = false
    private var sprint = false
    private val runSpeed = 50f
    private val walkSpeed = 10f

    private var velocity = Vector3f()

    override fun update() {
        mouseAction()
        rotate()
        move()
        updateCamera()
    }

    override fun render() {
    }

    private fun updateCamera() {
        Camera.position = position
        Camera.rotation = rotation
        Camera.update()
    }

    private fun rotate() {
        val mousePosition = Mouse.position

        var xOffset = mousePosition.x - lastMousePosition.x
        var yOffset = mousePosition.y - lastMousePosition.y

        lastMousePosition = mousePosition.copy()

        xOffset *= sensitivity
        yOffset *= sensitivity

        rotation.x += xOffset
        rotation.y += yOffset

        if (rotation.y > 89.0f) rotation.y = 89.0f
        if (rotation.y < -89.0f) rotation.y = -89.0f
    }

    private fun move() {
        moving = false

        if ((Keyboard.isKeyDown(GLFW.GLFW_KEY_LEFT_CONTROL))) {
            sprint = true
        }

        if ((Keyboard.isKeyDown(GLFW.GLFW_KEY_W))) {
            velocity.x -= sin(-rotation.x * Math.PI / 180).toFloat()
            velocity.z -= cos(-rotation.x * Math.PI / 180).toFloat()
            moving = true
        }

        if ((Keyboard.isKeyDown(GLFW.GLFW_KEY_S))) {
            velocity.x += sin(-rotation.x * Math.PI / 180).toFloat()
            velocity.z += cos(-rotation.x * Math.PI / 180).toFloat()
            moving = true
        }

        if ((Keyboard.isKeyDown(GLFW.GLFW_KEY_A))) {
            velocity.x += sin((-rotation.x - 90) * Math.PI / 180).toFloat()
            velocity.z += cos((-rotation.x - 90) * Math.PI / 180).toFloat()
            moving = true
        }

        if ((Keyboard.isKeyDown(GLFW.GLFW_KEY_D))) {
            velocity.x += sin((-rotation.x + 90) * Math.PI / 180).toFloat()
            velocity.z += cos((-rotation.x + 90) * Math.PI / 180).toFloat()
            moving = true
        }

        if (Keyboard.isKeyDown(GLFW.GLFW_KEY_SPACE)) {
            velocity.y += 1
            moving = true
        }

        if (Keyboard.isKeyDown(GLFW.GLFW_KEY_LEFT_SHIFT)) {
            velocity.y -= 1
            moving = true
        }

        val speed = if (sprint) runSpeed else walkSpeed


        if (moving) {
            val horizontalVelocity = Vector3f(velocity.x, 0f, velocity.z)

            velocity.x = horizontalVelocity.normalize().x
            velocity.z = horizontalVelocity.normalize().z
        } else {
            sprint = false
        }

        position.x += velocity.x * Time.delta.toFloat() * speed
        position.y += velocity.y * Time.delta.toFloat() * speed
        position.z += velocity.z * Time.delta.toFloat() * speed

        velocity.zero()
    }

    private fun mouseAction() {
        if (Mouse.isButtonPressed(Mouse.RIGHT_BUTTON)) {
        }

        if (Mouse.isButtonPressed(Mouse.LEFT_BUTTON)) {
        }

        if (Mouse.isButtonPressed(Mouse.MIDDLE_BUTTON)) {
        }
    }
}
