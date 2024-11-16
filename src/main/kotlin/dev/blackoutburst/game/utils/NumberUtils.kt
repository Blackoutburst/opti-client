package dev.blackoutburst.game.utils

import dev.blackoutburst.game.maths.Vector2f
import dev.blackoutburst.game.maths.Vector3f
import java.text.DecimalFormat
import java.text.DecimalFormatSymbols
import java.util.*
import kotlin.math.exp
import kotlin.math.pow

val formatter = DecimalFormat("#,###", DecimalFormatSymbols(Locale.getDefault()).apply { groupingSeparator = ' ' })

fun FBN(number: Number): String = formatter.format(number)

fun expDecay(a: Float, b: Float, decay: Float, delta: Double): Float = (b + (a - b) * exp(-decay * delta)).toFloat()

fun distance(p1: Vector2f, p2: Vector2f): Double =
    (p2.x.toDouble() - p1.x.toDouble()).pow(2.0) +
            (p2.y.toDouble() - p1.y.toDouble()).pow(2.0)

fun distance(p1: Vector3f, p2: Vector3f): Double =
    (p2.x.toDouble() - p1.x.toDouble()).pow(2.0) +
    (p2.y.toDouble() - p1.y.toDouble()).pow(2.0) +
    (p2.z.toDouble() - p1.z.toDouble()).pow(2.0)