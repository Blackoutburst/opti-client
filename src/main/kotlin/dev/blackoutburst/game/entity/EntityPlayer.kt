package dev.blackoutburst.game.entity

import dev.blackoutburst.game.Main
import dev.blackoutburst.game.camera.Camera
import dev.blackoutburst.game.input.Keyboard
import dev.blackoutburst.game.input.Mouse
import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.network.Connection
import dev.blackoutburst.game.network.packets.client.C00UpdateEntity
import dev.blackoutburst.game.network.packets.client.C01UpdateBlock
import dev.blackoutburst.game.utils.Time
import dev.blackoutburst.game.world.BlockType
import dev.blackoutburst.game.world.World
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
        networkUpdate()
    }

    private fun networkUpdate() {
        if (Time.doUpdate()) {
            Connection.write(C00UpdateEntity(id, position, rotation))
        }
    }

    override fun render(view: Matrix, projection: Matrix) {
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
            val result = World.dda(Camera.position, Camera.direction, 100)
            result.block?.let { b ->
                result.face?.let { f ->
                    //alSourcePlay(Main.source2)
                    Connection.write(C01UpdateBlock(Main.blockType.id, b.position + f))
                }
            }
        }

        if (Mouse.isButtonPressed(Mouse.LEFT_BUTTON)) {
            World.dda(Camera.position, Camera.direction, 100)
                .block?.let {
                    //alSourcePlay(Main.source)
                    Connection.write(C01UpdateBlock(BlockType.AIR.id, it.position))
                }
        }

        if (Mouse.isButtonPressed(Mouse.MIDDLE_BUTTON)) {
            World.dda(Camera.position, Camera.direction, 100).block?.let {
                Main.blockType = it.type
            }
        }
    }
}
