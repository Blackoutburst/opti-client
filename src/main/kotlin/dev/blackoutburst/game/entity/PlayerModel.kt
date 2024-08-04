package dev.blackoutburst.game.entity

abstract class PlayerModel(
    val texture: Int,
) {
    open val vaos: MutableMap<String, Int> = mutableMapOf()
    open val indices: Map<String, List<Int>> = mutableMapOf()
}