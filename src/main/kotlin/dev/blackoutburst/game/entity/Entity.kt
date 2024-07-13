package dev.blackoutburst.game.entity

import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f

abstract class Entity(
    val id: Int,
    var position: Vector3f,
    var rotation: Vector2f,
    var rawPosition: Vector3f = Vector3f(),
    var rawRotation: Vector2f = Vector2f(),
    var previousRawPosition: Vector3f = Vector3f(),
    var previousRawRotation: Vector2f = Vector2f(),
) {
    abstract fun update()
    abstract fun render(view: Matrix, projection: Matrix)
}