package dev.blackoutburst.game.utils

import java.text.DecimalFormat
import java.text.DecimalFormatSymbols
import java.util.*
import kotlin.math.exp

val formatter = DecimalFormat("#,###", DecimalFormatSymbols(Locale.getDefault()).apply { groupingSeparator = ' ' })

fun FBN(number: Number): String = formatter.format(number)

fun expDecay(a: Float, b: Float, decay: Float, delta: Double): Float = (b + (a - b) * exp(-decay * delta)).toFloat()
