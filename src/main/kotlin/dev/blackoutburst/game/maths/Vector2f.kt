package dev.blackoutburst.game.maths

import dev.blackoutburst.game.utils.Time
import dev.blackoutburst.game.utils.expDecay
import kotlin.math.sqrt

class Vector2f {
    var x: Float
    var y: Float

    constructor() {
        this.x = 0.0f
        this.y = 0.0f
    }

    constructor(size: Float) {
        this.x = size
        this.y = size
    }

    constructor(x: Float, y: Float) {
        this.x = x
        this.y = y
    }

    fun set(x: Float, y: Float) {
        this.x = x
        this.y = y
    }

    fun normalize(): Vector2f {
        val mag = sqrt((x * x + y * y).toDouble())
        if (mag != 0.0) {
            x /= mag.toFloat()
            y /= mag.toFloat()
        }

        return (this)
    }

    fun mul(v: Float): Vector2f {
        x *= v
        y *= v

        return (this)
    }

    fun length(): Float {
        return (sqrt((x * x + y * y).toDouble()).toFloat())
    }

    fun copy(): Vector2f {
        val newVector = Vector2f()
        newVector.x = this.x
        newVector.y = this.y

        return (newVector)
    }

    fun lerp(target: Vector2f, decay: Float): Vector2f {
        val newX = expDecay(this.x, target.x, decay, Time.delta)
        val newY = expDecay(this.y, target.y, decay, Time.delta)
        return Vector2f(newX, newY)
    }

    override fun toString(): String {
        return "[$x, $y]"
    }

    operator fun minus(other: Vector2f) = Vector2f(x - other.x, y - other.y)
}
