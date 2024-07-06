package dev.blackoutburst.game.entity

import dev.blackoutburst.game.Cube
import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import kotlin.math.cos
import kotlin.math.sin

class EntityOtherPlayer(
    id: Int,
    position: Vector3f = Vector3f(),
    rotation: Vector2f = Vector2f(),
) : Entity(id, position, rotation) {

    val cube = Cube()

    override fun update() {
    }

    override fun render() {
        val xRad = -rotation.x.toDouble().toFloat()
        val yRad = -rotation.y.toDouble().toFloat()

        cube.render(
            Matrix()
                .translate(position)
                .rotate(yRad, Vector3f(cos(xRad), 0f, -sin(xRad)))
                .rotate(xRad, Vector3f(0f, 1f, 0f))
        )
    }
}