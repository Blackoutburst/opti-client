package dev.blackoutburst.game.entity

import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.network.Connection
import java.util.*
import kotlin.collections.LinkedHashSet

object EntityManager {
    private val entities: MutableSet<Entity> = Collections.synchronizedSet(LinkedHashSet())

    fun addEntity(entity: Entity) {
        if (entity.id == Connection.id) return

        entities.add(entity)
    }

    fun update() {
        entities.forEach {
            if (it.id != Connection.id) {
                it.position = it.position.lerp(it.rawPosition, 50f)
                it.rotation = it.rotation.lerp(it.rawRotation, 50f)
            }
            it.update()
        }
    }

    fun render(view: Matrix, projection: Matrix) {
        entities.forEach { it.render(view, projection) }
    }

    fun setPosition(id: Int, position: Vector3f) {
        if (id == Connection.id) return

        entities.find { it.id == id }?.let {
            it.previousRawPosition = it.rawPosition
            it.rawPosition = position
        }
    }

    fun setRotation(id: Int, rotation: Vector2f) {
        if (id == Connection.id) return

        entities.find { it.id == id }?.let {
            it.previousRawRotation = it.rawRotation
            it.rawRotation = rotation
        }
    }

    fun removeEntity(id: Int) {
        entities.find { it.id == id }?.let {
            entities.remove(it)
        }
    }
}