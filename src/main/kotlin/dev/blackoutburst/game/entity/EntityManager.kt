package dev.blackoutburst.game.entity

import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import dev.blackoutburst.game.network.Connection
import dev.blackoutburst.game.utils.SkinAPI
import java.util.*
import kotlin.collections.LinkedHashSet
import kotlin.math.round

object EntityManager {
    private val entities: MutableSet<Entity> = Collections.synchronizedSet(LinkedHashSet())

    fun addEntity(entity: Entity) {
        if (entity.id == Connection.id) return

        entities.add(entity)
    }

    fun update() {
        entities.forEach {
            if (it.id != Connection.id) {
                it.position = it.position.lerp(it.rawPosition, 10f)
                it.rotation = it.rotation.lerp(it.rawRotation, 10f)
            }
            it.update()
        }
    }

    fun render(view: Matrix, projection: Matrix) {
        entities.forEach { it.render(view, projection) }
    }

    fun setPosition(id: Int, position: Vector3f) {
        entities.find { it.id == id }?.let {
            if (it is EntityPlayer) {
                it.position = position
            } else {
                it.previousRawPosition = it.rawPosition
                it.rawPosition = position
            }
        }
    }

    fun setName(id: Int, name: String) {
        entities.find { it.id == id }?.let {
            it.name = name
        }
    }

    fun setRotation(id: Int, rotation: Vector2f) {
        entities.find { it.id == id }?.let {
            if (it is EntityPlayer) {
                it.rotation = rotation
            } else {
                it.previousRawRotation = it.rawRotation
                it.rawRotation = rotation
            }
        }
    }

    fun removeEntity(id: Int) {
        entities.find { it.id == id }?.let {
            entities.remove(it)
        }
    }
}