package dev.blackoutburst.game.entity

import dev.blackoutburst.game.utils.stack
import org.lwjgl.opengl.GL30.*

object PlayerModelFemale {

    val vaos = mutableMapOf(
        "head" to 0,
        "body" to 0,
        "rightArm" to 0,
        "leftArm" to 0,
        "rightLeg" to 0,
        "leftLeg" to 0,
    )

    private fun uv(n1: Float, n2: Float, n3: Float): Float = if (n1 == 0f) n2 / 64f else n3 / 64f

    val indices = intArrayOf(
        //TOP
        0, 2, 1,
        0, 3, 2,

        //FRONT
        4, 6, 5,
        4, 7, 6,

        //BACK
        9, 10, 8,
        10, 11, 8,

        //LEFT
        12, 14, 13,
        12, 15, 14,

        //RIGHT
        17, 18, 16,
        18, 19, 16,

        //BOTTOM
        21, 22, 20,
        22, 23, 20,

        //TOP
        0 + 24, 2 + 24, 1 + 24,
        0 + 24, 3 + 24, 2 + 24,

        //FRONT
        4 + 24, 6 + 24, 5 + 24,
        4 + 24, 7 + 24, 6 + 24,

        //BACK
        9 + 24, 10 + 24, 8 + 24,
        10 + 24, 11 + 24, 8 + 24,

        //LEFT
        12 + 24, 14 + 24, 13 + 24,
        12 + 24, 15 + 24, 14 + 24,

        //RIGHT
        17 + 24, 18 + 24, 16 + 24,
        18 + 24, 19 + 24, 16 + 24,

        //BOTTOM
        21 + 24, 22 + 24, 20 + 24,
        22 + 24, 23 + 24, 20 + 24,
    )

    private val headVertices = floatArrayOf(
        //TOP
        0f - 0.01f, 1f + 0.01f, 0f - 0.01f, uv(1f,8f,16f), uv(1f,0f,8f), 0f,
        1f + 0.01f, 1f + 0.01f, 0f - 0.01f, uv(0f,8f,16f), uv(1f,0f,8f), 0f,
        1f + 0.01f, 1f + 0.01f, 1f + 0.01f, uv(0f,8f,16f), uv(0f,0f,8f), 0f,
        0f - 0.01f, 1f + 0.01f, 1f + 0.01f, uv(1f,8f,16f), uv(0f,0f,8f), 0f,

        //FRONT
        0f - 0.01f, 0f - 0.01f, 0f - 0.01f, uv(1f,8f,16f), uv(1f,8f,16f), 1f,
        1f + 0.01f, 0f - 0.01f, 0f - 0.01f, uv(0f,8f,16f), uv(1f,8f,16f), 1f,
        1f + 0.01f, 1f + 0.01f, 0f - 0.01f, uv(0f,8f,16f), uv(0f,8f,16f), 1f,
        0f - 0.01f, 1f + 0.01f, 0f - 0.01f, uv(1f,8f,16f), uv(0f,8f,16f), 1f,

        //BACK
        0f - 0.01f, 0f - 0.01f, 1f + 0.01f, uv(0f,24f,32f), uv(1f,8f,16f), 2f,
        1f + 0.01f, 0f - 0.01f, 1f + 0.01f, uv(1f,24f,32f), uv(1f,8f,16f), 2f,
        1f + 0.01f, 1f + 0.01f, 1f + 0.01f, uv(1f,24f,32f), uv(0f,8f,16f), 2f,
        0f - 0.01f, 1f + 0.01f, 1f + 0.01f, uv(0f,24f,32f), uv(0f,8f,16f), 2f,

        //LEFT
        0f - 0.01f, 0f - 0.01f, 0f - 0.01f, uv(0f,16f,24f), uv(1f,8f,16f), 3f,
        0f - 0.01f, 1f + 0.01f, 0f - 0.01f, uv(0f,16f,24f), uv(0f,8f,16f), 3f,
        0f - 0.01f, 1f + 0.01f, 1f + 0.01f, uv(1f,16f,24f), uv(0f,8f,16f), 3f,
        0f - 0.01f, 0f - 0.01f, 1f + 0.01f, uv(1f,16f,24f), uv(1f,8f,16f), 3f,

        //RIGHT
        1f + 0.01f, 0f - 0.01f, 0f - 0.01f, uv(1f,0f,8f), uv(1f,8f,16f), 4f,
        1f + 0.01f, 1f + 0.01f, 0f - 0.01f, uv(1f,0f,8f), uv(0f,8f,16f), 4f,
        1f + 0.01f, 1f + 0.01f, 1f + 0.01f, uv(0f,0f,8f), uv(0f,8f,16f), 4f,
        1f + 0.01f, 0f - 0.01f, 1f + 0.01f, uv(0f,0f,8f), uv(1f,8f,16f), 4f,

        //BOTTOM
        0f - 0.01f, 0f - 0.01f, 0f - 0.01f, uv(0f,16f,24f), uv(1f,0f,8f), 5f,
        1f + 0.01f, 0f - 0.01f, 0f - 0.01f, uv(1f,16f,24f), uv(1f,0f,8f), 5f,
        1f + 0.01f, 0f - 0.01f, 1f + 0.01f, uv(1f,16f,24f), uv(0f,0f,8f), 5f,
        0f - 0.01f, 0f - 0.01f, 1f + 0.01f, uv(0f,16f,24f), uv(0f,0f,8f), 5f,
    )

    private val headLayerVertices = floatArrayOf(
        //TOP
        0f - 0.03f, 1f + 0.03f, 0f - 0.03f, uv(1f,40f,48f), uv(1f,0f,8f), 0f,
        1f + 0.03f, 1f + 0.03f, 0f - 0.03f, uv(0f,40f,48f), uv(1f,0f,8f), 0f,
        1f + 0.03f, 1f + 0.03f, 1f + 0.03f, uv(0f,40f,48f), uv(0f,0f,8f), 0f,
        0f - 0.03f, 1f + 0.03f, 1f + 0.03f, uv(1f,40f,48f), uv(0f,0f,8f), 0f,

        //FRONT
        0f - 0.03f, 0f - 0.03f, 0f - 0.03f, uv(1f,40f,48f), uv(1f,8f,16f), 1f,
        1f + 0.03f, 0f - 0.03f, 0f - 0.03f, uv(0f,40f,48f), uv(1f,8f,16f), 1f,
        1f + 0.03f, 1f + 0.03f, 0f - 0.03f, uv(0f,40f,48f), uv(0f,8f,16f), 1f,
        0f - 0.03f, 1f + 0.03f, 0f - 0.03f, uv(1f,40f,48f), uv(0f,8f,16f), 1f,

        //BACK
        0f - 0.03f, 0f - 0.03f, 1f + 0.03f, uv(0f,56f,64f), uv(1f,8f,16f), 2f,
        1f + 0.03f, 0f - 0.03f, 1f + 0.03f, uv(1f,56f,64f), uv(1f,8f,16f), 2f,
        1f + 0.03f, 1f + 0.03f, 1f + 0.03f, uv(1f,56f,64f), uv(0f,8f,16f), 2f,
        0f - 0.03f, 1f + 0.03f, 1f + 0.03f, uv(0f,56f,64f), uv(0f,8f,16f), 2f,

        //LEFT
        0f - 0.03f, 0f - 0.03f, 0f - 0.03f, uv(0f,48f,56f), uv(1f,8f,16f), 3f,
        0f - 0.03f, 1f + 0.03f, 0f - 0.03f, uv(0f,48f,56f), uv(0f,8f,16f), 3f,
        0f - 0.03f, 1f + 0.03f, 1f + 0.03f, uv(1f,48f,56f), uv(0f,8f,16f), 3f,
        0f - 0.03f, 0f - 0.03f, 1f + 0.03f, uv(1f,48f,56f), uv(1f,8f,16f), 3f,

        //RIGHT
        1f + 0.03f, 0f - 0.03f, 0f - 0.03f, uv(1f,32f,40f), uv(1f,8f,16f), 4f,
        1f + 0.03f, 1f + 0.03f, 0f - 0.03f, uv(1f,32f,40f), uv(0f,8f,16f), 4f,
        1f + 0.03f, 1f + 0.03f, 1f + 0.03f, uv(0f,32f,40f), uv(0f,8f,16f), 4f,
        1f + 0.03f, 0f - 0.03f, 1f + 0.03f, uv(0f,32f,40f), uv(1f,8f,16f), 4f,

        //BOTTOM
        0f - 0.03f, 0f - 0.03f, 0f - 0.03f, uv(0f,48f,56f), uv(1f,0f,8f), 5f,
        1f + 0.03f, 0f - 0.03f, 0f - 0.03f, uv(1f,48f,56f), uv(1f,0f,8f), 5f,
        1f + 0.03f, 0f - 0.03f, 1f + 0.03f, uv(1f,48f,56f), uv(0f,0f,8f), 5f,
        0f - 0.03f, 0f - 0.03f, 1f + 0.03f, uv(0f,48f,56f), uv(0f,0f,8f), 5f,
    )

    private val bodyVertices = floatArrayOf(
        //TOP
        (0f), (1f - 1f), (0f + 0.25f),                uv(1f,20f,28f), uv(1f,16f,20f), 0f,
        (1f), (1f - 1f), (0f + 0.25f),                uv(0f,20f,28f), uv(1f,16f,20f), 0f,
        (1f), (1f - 1f), (1f - 0.25f),                uv(0f,20f,28f), uv(0f,16f,20f), 0f,
        (0f), (1f - 1f), (1f - 0.25f),                uv(1f,20f,28f), uv(0f,16f,20f), 0f,

        //FRONT
        (0f), (0f - 1f - 0.5f), (0f + 0.25f),         uv(1f,20f,28f), uv(1f,20f,32f), 1f,
        (1f), (0f - 1f - 0.5f), (0f + 0.25f),         uv(0f,20f,28f), uv(1f,20f,32f), 1f,
        (1f), (1f - 1f), (0f + 0.25f),                uv(0f,20f,28f), uv(0f,20f,32f), 1f,
        (0f), (1f - 1f), (0f + 0.25f),                uv(1f,20f,28f), uv(0f,20f,32f), 1f,

        //BACK
        (0f), (0f - 1f - 0.5f), (1f - 0.25f),         uv(0f,32f,40f), uv(1f,20f,32f), 2f,
        (1f), (0f - 1f - 0.5f), (1f - 0.25f),         uv(1f,32f,40f), uv(1f,20f,32f), 2f,
        (1f), (1f - 1f), (1f - 0.25f),                uv(1f,32f,40f), uv(0f,20f,32f), 2f,
        (0f), (1f - 1f), (1f - 0.25f),                uv(0f,32f,40f), uv(0f,20f,32f), 2f,

        //LEFT
        (0f), (0f - 1f - 0.5f), (0f + 0.25f),         uv(0f,28f,32f), uv(1f,20f,32f), 3f,
        (0f), (1f - 1f), (0f + 0.25f),                uv(0f,28f,32f), uv(0f,20f,32f), 3f,
        (0f), (1f - 1f), (1f - 0.25f),                uv(1f,28f,32f), uv(0f,20f,32f), 3f,
        (0f), (0f - 1f - 0.5f), (1f - 0.25f),         uv(1f,28f,32f), uv(1f,20f,32f), 3f,

        //RIGHT
        (1f), (0f - 1f - 0.5f), (0f + 0.25f),         uv(1f,16f,20f), uv(1f,20f,32f), 4f,
        (1f), (1f - 1f), (0f + 0.25f),                uv(1f,16f,20f), uv(0f,20f,32f), 4f,
        (1f), (1f - 1f), (1f - 0.25f),                uv(0f,16f,20f), uv(0f,20f,32f), 4f,
        (1f), (0f - 1f - 0.5f), (1f - 0.25f),         uv(0f,16f,20f), uv(1f,20f,32f), 4f,

        //BOTTOM
        (0f), (0f - 1f - 0.5f), (0f + 0.25f),         uv(0f,28f,36f), uv(1f,16f,20f), 5f,
        (1f), (0f - 1f - 0.5f), (0f + 0.25f),         uv(1f,28f,36f), uv(1f,16f,20f), 5f,
        (1f), (0f - 1f - 0.5f), (1f - 0.25f),         uv(1f,28f,36f), uv(0f,16f,20f), 5f,
        (0f), (0f - 1f - 0.5f), (1f - 0.25f),         uv(0f,28f,36f), uv(0f,16f,20f), 5f,
    )

    private val bodyLayerVertices = floatArrayOf(
        //TOP
        (0f - 0.031f), (1f - 1f + 0.031f), (0f + 0.25f - 0.031f),                uv(1f,20f,28f), uv(1f,32f,36f), 0f,
        (1f + 0.031f), (1f - 1f + 0.031f), (0f + 0.25f - 0.031f),                uv(0f,20f,28f), uv(1f,32f,36f), 0f,
        (1f + 0.031f), (1f - 1f + 0.031f), (1f - 0.25f + 0.031f),                uv(0f,20f,28f), uv(0f,32f,36f), 0f,
        (0f - 0.031f), (1f - 1f + 0.031f), (1f - 0.25f + 0.031f),                uv(1f,20f,28f), uv(0f,32f,36f), 0f,

        //FRONT
        (0f - 0.031f), (0f - 1f - 0.5f - 0.031f), (0f + 0.25f - 0.031f),         uv(1f,20f,28f), uv(1f,36f,48f), 1f,
        (1f + 0.031f), (0f - 1f - 0.5f - 0.031f), (0f + 0.25f - 0.031f),         uv(0f,20f,28f), uv(1f,36f,48f), 1f,
        (1f + 0.031f), (1f - 1f + 0.031f), (0f + 0.25f - 0.031f),                uv(0f,20f,28f), uv(0f,36f,48f), 1f,
        (0f - 0.031f), (1f - 1f + 0.031f), (0f + 0.25f - 0.031f),                uv(1f,20f,28f), uv(0f,36f,48f), 1f,

        //BACK
        (0f - 0.031f), (0f - 1f - 0.5f - 0.031f), (1f - 0.25f + 0.031f),         uv(0f,32f,40f), uv(1f,36f,48f), 2f,
        (1f + 0.031f), (0f - 1f - 0.5f - 0.031f), (1f - 0.25f + 0.031f),         uv(1f,32f,40f), uv(1f,36f,48f), 2f,
        (1f + 0.031f), (1f - 1f + 0.031f), (1f - 0.25f + 0.031f),                uv(1f,32f,40f), uv(0f,36f,48f), 2f,
        (0f - 0.031f), (1f - 1f + 0.031f), (1f - 0.25f + 0.031f),                uv(0f,32f,40f), uv(0f,36f,48f), 2f,

        //LEFT
        (0f - 0.031f), (0f - 1f - 0.5f - 0.031f), (0f + 0.25f - 0.031f),         uv(0f,28f,32f), uv(1f,36f,48f), 3f,
        (0f - 0.031f), (1f - 1f + 0.031f), (0f + 0.25f - 0.031f),                uv(0f,28f,32f), uv(0f,36f,48f), 3f,
        (0f - 0.031f), (1f - 1f + 0.031f), (1f - 0.25f + 0.031f),                uv(1f,28f,32f), uv(0f,36f,48f), 3f,
        (0f - 0.031f), (0f - 1f - 0.5f - 0.031f), (1f - 0.25f + 0.031f),         uv(1f,28f,32f), uv(1f,36f,48f), 3f,

        //RIGHT
        (1f + 0.031f), (0f - 1f - 0.5f - 0.031f), (0f + 0.25f - 0.031f),         uv(1f,16f,20f), uv(1f,36f,48f), 4f,
        (1f + 0.031f), (1f - 1f + 0.031f), (0f + 0.25f - 0.031f),                uv(1f,16f,20f), uv(0f,36f,48f), 4f,
        (1f + 0.031f), (1f - 1f + 0.031f), (1f - 0.25f + 0.031f),                uv(0f,16f,20f), uv(0f,36f,48f), 4f,
        (1f + 0.031f), (0f - 1f - 0.5f - 0.031f), (1f - 0.25f + 0.031f),         uv(0f,16f,20f), uv(1f,36f,48f), 4f,

        //BOTTOM
        (0f - 0.031f), (0f - 1f - 0.5f - 0.031f), (0f + 0.25f - 0.031f),         uv(0f,28f,36f), uv(1f,32f,36f), 5f,
        (1f + 0.031f), (0f - 1f - 0.5f - 0.031f), (0f + 0.25f - 0.031f),         uv(1f,28f,36f), uv(1f,32f,36f), 5f,
        (1f + 0.031f), (0f - 1f - 0.5f - 0.031f), (1f - 0.25f + 0.031f),         uv(1f,28f,36f), uv(0f,32f,36f), 5f,
        (0f - 0.031f), (0f - 1f - 0.5f - 0.031f), (1f - 0.25f + 0.03f),         uv(0f,28f,36f), uv(0f,32f,36f), 5f,
    )

    private val rightArmVertices = floatArrayOf(
        //TOP
        ((0f + 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(1f,44f,47f), uv(1f,16f,20f), 0f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(0f,44f,47f), uv(1f,16f,20f), 0f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(0f,44f,47f), uv(0f,16f,20f), 0f,
        ((0f + 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(1f,44f,47f), uv(0f,16f,20f), 0f,

        //FRONT
        ((0f + 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(1f,44f,47f), uv(1f,20f,32f), 1f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(0f,44f,47f), uv(1f,20f,32f), 1f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(0f,44f,47f), uv(0f,20f,32f), 1f,
        ((0f + 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(1f,44f,47f), uv(0f,20f,32f), 1f,

        //BACK
        ((0f + 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(0f,51f,54f), uv(1f,20f,32f), 2f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(1f,51f,54f), uv(1f,20f,32f), 2f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(1f,51f,54f), uv(0f,20f,32f), 2f,
        ((0f + 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(0f,51f,54f), uv(0f,20f,32f), 2f,

        //LEFT
        ((0f + 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(0f,47f,51f), uv(1f,20f,32f), 3f,
        ((0f + 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(0f,47f,51f), uv(0f,20f,32f), 3f,
        ((0f + 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(1f,47f,51f), uv(0f,20f,32f), 3f,
        ((0f + 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(1f,47f,51f), uv(1f,20f,32f), 3f,

        //RIGHT
        ((1f - 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(1f,40f,44f), uv(1f,20f,32f), 4f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(1f,40f,44f), uv(0f,20f,32f), 4f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(0f,40f,44f), uv(0f,20f,32f), 4f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(0f,40f,44f), uv(1f,20f,32f), 4f,

        //BOTTOM
        ((0f + 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(1f,47f,50f), uv(1f,16f,20f), 5f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(0f,47f,50f), uv(1f,16f,20f), 5f,
        ((1f - 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(0f,47f,50f), uv(0f,16f,20f), 5f,
        ((0f + 0.3125f) + 0.75f - 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(1f,47f,50f), uv(0f,16f,20f), 5f,
    )

    private val rightArmLayerVertices = floatArrayOf(
        //TOP
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,44f,47f), uv(1f,32f,36f), 0f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,44f,47f), uv(1f,32f,36f), 0f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,44f,47f), uv(0f,32f,36f), 0f,
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,44f,47f), uv(0f,32f,36f), 0f,

        //FRONT
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,44f,47f), uv(1f,36f,48f), 1f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,44f,47f), uv(1f,36f,48f), 1f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,44f,47f), uv(0f,36f,48f), 1f,
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,44f,47f), uv(0f,36f,48f), 1f,

        //BACK
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,51f,54f), uv(1f,36f,48f), 2f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,51f,54f), uv(1f,36f,48f), 2f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,51f,54f), uv(0f,36f,48f), 2f,
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,51f,54f), uv(0f,36f,48f), 2f,

        //LEFT
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,47f,51f), uv(1f,36f,48f), 3f,
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,47f,51f), uv(0f,36f,48f), 3f,
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,47f,51f), uv(0f,36f,48f), 3f,
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,47f,51f), uv(1f,36f,48f), 3f,

        //RIGHT
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,40f,44f), uv(1f,36f,48f), 4f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,40f,44f), uv(0f,36f,48f), 4f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,40f,44f), uv(0f,36f,48f), 4f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,40f,44f), uv(1f,36f,48f), 4f,

        //BOTTOM
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,47f,50f), uv(1f,32f,36f), 5f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,47f,50f), uv(1f,32f,36f), 5f,
        ((1f - 0.3125f) + 0.75f - 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,47f,50f), uv(0f,32f,36f), 5f,
        ((0f + 0.3125f) + 0.75f - 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,47f,50f), uv(0f,32f,36f), 5f,
    )

    private val leftArmVertices = floatArrayOf(
        //TOP
        ((0f + 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(1f,36f,39f), uv(1f,48f,52f), 0f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(0f,36f,39f), uv(1f,48f,52f), 0f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(0f,36f,39f), uv(0f,48f,52f), 0f,
        ((0f + 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(1f,36f,39f), uv(0f,48f,52f), 0f,

        //FRONT
        ((0f + 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(1f,36f,39f), uv(1f,52f,64f), 1f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(0f,36f,39f), uv(1f,52f,64f), 1f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(0f,36f,39f), uv(0f,52f,64f), 1f,
        ((0f + 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(1f,36f,39f), uv(0f,52f,64f), 1f,

        //BACK
        ((0f + 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(0f,43f,46f), uv(1f,52f,64f), 2f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(1f,43f,46f), uv(1f,52f,64f), 2f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(1f,43f,46f), uv(0f,52f,64f), 2f,
        ((0f + 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(0f,43f,46f), uv(0f,52f,64f), 2f,

        //LEFT
        ((0f + 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(0f,39f,43f), uv(1f,52f,64f), 3f,
        ((0f + 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(0f,39f,43f), uv(0f,52f,64f), 3f,
        ((0f + 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(1f,39f,43f), uv(0f,52f,64f), 3f,
        ((0f + 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(1f,39f,43f), uv(1f,52f,64f), 3f,

        //RIGHT
        ((1f - 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(1f,32f,36f), uv(1f,52f,64f), 4f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (0f + 0.25f),                     uv(1f,32f,36f), uv(0f,52f,64f), 4f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (1f - 1f), (1f - 0.25f),                     uv(0f,32f,36f), uv(0f,52f,64f), 4f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(0f,32f,36f), uv(1f,52f,64f), 4f,

        //BOTTOM
        ((0f + 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(1f,39f,42f), uv(1f,48f,52f), 5f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (0f + 0.25f),              uv(0f,39f,42f), uv(1f,48f,52f), 5f,
        ((1f - 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(0f,39f,42f), uv(0f,48f,52f), 5f,
        ((0f + 0.3125f) - 0.75f + 0.0625f), (0f - 1f - 0.5f), (1f - 0.25f),              uv(1f,39f,42f), uv(0f,48f,52f), 5f,
    )

    private val leftArmLayerVertices = floatArrayOf(
        //TOP
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,52f,55f), uv(1f,48f,52f), 0f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,52f,55f), uv(1f,48f,52f), 0f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,52f,55f), uv(0f,48f,52f), 0f,
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,52f,55f), uv(0f,48f,52f), 0f,

        //FRONT
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,52f,55f), uv(1f,52f,64f), 1f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,52f,55f), uv(1f,52f,64f), 1f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,52f,55f), uv(0f,52f,64f), 1f,
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,52f,55f), uv(0f,52f,64f), 1f,

        //BACK
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,59f,62f), uv(1f,52f,64f), 2f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,59f,62f), uv(1f,52f,64f), 2f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,59f,62f), uv(0f,52f,64f), 2f,
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,59f,62f), uv(0f,52f,64f), 2f,

        //LEFT
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,55f,59f), uv(1f,52f,64f), 3f,
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,55f,59f), uv(0f,52f,64f), 3f,
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,55f,59f), uv(0f,52f,64f), 3f,
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,55f,59f), uv(1f,52f,64f), 3f,

        //RIGHT
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,48f,52f), uv(1f,52f,64f), 4f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (1f - 1f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,48f,52f), uv(0f,52f,64f), 4f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (1f - 1f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,48f,52f), uv(0f,52f,64f), 4f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,48f,52f), uv(1f,52f,64f), 4f,

        //BOTTOM
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,55f,58f), uv(1f,48f,52f), 5f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,55f,58f), uv(1f,48f,52f), 5f,
        ((1f - 0.3125f) - 0.75f + 0.0625f + 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,55f,58f), uv(0f,48f,52f), 5f,
        ((0f + 0.3125f) - 0.75f + 0.0625f - 0.03f), (0f - 1f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,55f,58f), uv(0f,48f,52f), 5f,
    )

    private val rightLegVertices = floatArrayOf(
        //TOP
        ((0f + 0.25f) + 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(1f,4f,8f), uv(1f,16f,20f), 0f,
        ((1f - 0.25f) + 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(0f,4f,8f), uv(1f,16f,20f), 0f,
        ((1f - 0.25f) + 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(0f,4f,8f), uv(0f,16f,20f), 0f,
        ((0f + 0.25f) + 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(1f,4f,8f), uv(0f,16f,20f), 0f,

        //FRONT
        ((0f + 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(1f,4f,8f), uv(1f,20f,32f), 1f,
        ((1f - 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(0f,4f,8f), uv(1f,20f,32f), 1f,
        ((1f - 0.25f) + 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(0f,4f,8f), uv(0f,20f,32f), 1f,
        ((0f + 0.25f) + 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(1f,4f,8f), uv(0f,20f,32f), 1f,

        //BACK
        ((0f + 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(0f,12f,16f), uv(1f,20f,32f), 2f,
        ((1f - 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(1f,12f,16f), uv(1f,20f,32f), 2f,
        ((1f - 0.25f) + 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(1f,12f,16f), uv(0f,20f,32f), 2f,
        ((0f + 0.25f) + 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(0f,12f,16f), uv(0f,20f,32f), 2f,

        //LEFT
        ((0f + 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(0f,8f,12f), uv(1f,20f,32f), 3f,
        ((0f + 0.25f) + 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(0f,8f,12f), uv(0f,20f,32f), 3f,
        ((0f + 0.25f) + 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(1f,8f,12f), uv(0f,20f,32f), 3f,
        ((0f + 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(1f,8f,12f), uv(1f,20f,32f), 3f,

        //RIGHT
        ((1f - 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(1f,0f,4f), uv(1f,20f,32f), 4f,
        ((1f - 0.25f) + 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(1f,0f,4f), uv(0f,20f,32f), 4f,
        ((1f - 0.25f) + 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(0f,0f,4f), uv(0f,20f,32f), 4f,
        ((1f - 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(0f,0f,4f), uv(1f,20f,32f), 4f,

        //BOTTOM
        ((0f + 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(1f,8f,12f), uv(1f,16f,20f), 5f,
        ((1f - 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(0f,8f,12f), uv(1f,16f,20f), 5f,
        ((1f - 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(0f,8f,12f), uv(0f,16f,20f), 5f,
        ((0f + 0.25f) + 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(1f,8f,12f), uv(0f,16f,20f), 5f,
    )

    private val rightLegLayerVertices = floatArrayOf(
        //TOP
        ((0f + 0.25f) + 0.25f - 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,4f,8f), uv(1f,32f,36f), 0f,
        ((1f - 0.25f) + 0.25f + 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,4f,8f), uv(1f,32f,36f), 0f,
        ((1f - 0.25f) + 0.25f + 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,4f,8f), uv(0f,32f,36f), 0f,
        ((0f + 0.25f) + 0.25f - 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,4f,8f), uv(0f,32f,36f), 0f,

        //FRONT
        ((0f + 0.25f) + 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,4f,8f), uv(1f,36f,48f), 1f,
        ((1f - 0.25f) + 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,4f,8f), uv(1f,36f,48f), 1f,
        ((1f - 0.25f) + 0.25f + 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,4f,8f), uv(0f,36f,48f), 1f,
        ((0f + 0.25f) + 0.25f - 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,4f,8f), uv(0f,36f,48f), 1f,

        //BACK
        ((0f + 0.25f) + 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,12f,16f), uv(1f,36f,48f), 2f,
        ((1f - 0.25f) + 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,12f,16f), uv(1f,36f,48f), 2f,
        ((1f - 0.25f) + 0.25f + 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,12f,16f), uv(0f,36f,48f), 2f,
        ((0f + 0.25f) + 0.25f - 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,12f,16f), uv(0f,36f,48f), 2f,

        //LEFT
        ((0f + 0.25f) + 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,8f,12f), uv(1f,36f,48f), 3f,
        ((0f + 0.25f) + 0.25f - 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,8f,12f), uv(0f,36f,48f), 3f,
        ((0f + 0.25f) + 0.25f - 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,8f,12f), uv(0f,36f,48f), 3f,
        ((0f + 0.25f) + 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,8f,12f), uv(1f,36f,48f), 3f,

        //RIGHT
        ((1f - 0.25f) + 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,0f,4f), uv(1f,36f,48f), 4f,
        ((1f - 0.25f) + 0.25f + 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,0f,4f), uv(0f,36f,48f), 4f,
        ((1f - 0.25f) + 0.25f + 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,0f,4f), uv(0f,36f,48f), 4f,
        ((1f - 0.25f) + 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,0f,4f), uv(1f,36f,48f), 4f,

        //BOTTOM
        ((0f + 0.25f) + 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,8f,12f), uv(1f,32f,36f), 5f,
        ((1f - 0.25f) + 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,8f,12f), uv(1f,32f,36f), 5f,
        ((1f - 0.25f) + 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,8f,12f), uv(0f,32f,36f), 5f,
        ((0f + 0.25f) + 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,8f,12f), uv(0f,32f,36f), 5f,
    )

    private val leftLegVertices = floatArrayOf(
        //TOP
        ((0f + 0.25f) - 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(1f,20f,24f), uv(1f,48f,52f), 0f,
        ((1f - 0.25f) - 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(0f,20f,24f), uv(1f,48f,52f), 0f,
        ((1f - 0.25f) - 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(0f,20f,24f), uv(0f,48f,52f), 0f,
        ((0f + 0.25f) - 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(1f,20f,24f), uv(0f,48f,52f), 0f,

        //FRONT
        ((0f + 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(1f,20f,24f), uv(1f,52f,64f), 1f,
        ((1f - 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(0f,20f,24f), uv(1f,52f,64f), 1f,
        ((1f - 0.25f) - 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(0f,20f,24f), uv(0f,52f,64f), 1f,
        ((0f + 0.25f) - 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(1f,20f,24f), uv(0f,52f,64f), 1f,

        //BACK
        ((0f + 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(0f,28f,32f), uv(1f,52f,64f), 2f,
        ((1f - 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(1f,28f,32f), uv(1f,52f,64f), 2f,
        ((1f - 0.25f) - 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(1f,28f,32f), uv(0f,52f,64f), 2f,
        ((0f + 0.25f) - 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(0f,28f,32f), uv(0f,52f,64f), 2f,

        //LEFT
        ((0f + 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(0f,24f,28f), uv(1f,52f,64f), 3f,
        ((0f + 0.25f) - 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(0f,24f,28f), uv(0f,52f,64f), 3f,
        ((0f + 0.25f) - 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(1f,24f,28f), uv(0f,52f,64f), 3f,
        ((0f + 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(1f,24f,28f), uv(1f,52f,64f), 3f,

        //RIGHT
        ((1f - 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(1f,16f,20f), uv(1f,52f,64f), 4f,
        ((1f - 0.25f) - 0.25f), (1f - 2.5f), (0f + 0.25f),                     uv(1f,16f,20f), uv(0f,52f,64f), 4f,
        ((1f - 0.25f) - 0.25f), (1f - 2.5f), (1f - 0.25f),                     uv(0f,16f,20f), uv(0f,52f,64f), 4f,
        ((1f - 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(0f,16f,20f), uv(1f,52f,64f), 4f,

        //BOTTOM
        ((0f + 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(1f,24f,28f), uv(1f,48f,52f), 5f,
        ((1f - 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (0f + 0.25f),              uv(0f,24f,28f), uv(1f,48f,52f), 5f,
        ((1f - 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(0f,24f,28f), uv(0f,48f,52f), 5f,
        ((0f + 0.25f) - 0.25f), (0f - 2.5f - 0.5f), (1f - 0.25f),              uv(1f,24f,28f), uv(0f,48f,52f), 5f,
    )

    private val leftLegLayerVertices = floatArrayOf(
        //TOP
        ((0f + 0.25f) - 0.25f - 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,4f,8f), uv(1f,48f,52f), 0f,
        ((1f - 0.25f) - 0.25f + 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,4f,8f), uv(1f,48f,52f), 0f,
        ((1f - 0.25f) - 0.25f + 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,4f,8f), uv(0f,48f,52f), 0f,
        ((0f + 0.25f) - 0.25f - 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,4f,8f), uv(0f,48f,52f), 0f,

        //FRONT
        ((0f + 0.25f) - 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,4f,8f), uv(1f,52f,64f), 1f,
        ((1f - 0.25f) - 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,4f,8f), uv(1f,52f,64f), 1f,
        ((1f - 0.25f) - 0.25f + 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,4f,8f), uv(0f,52f,64f), 1f,
        ((0f + 0.25f) - 0.25f - 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,4f,8f), uv(0f,52f,64f), 1f,

        //BACK
        ((0f + 0.25f) - 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,12f,16f), uv(1f,52f,64f), 2f,
        ((1f - 0.25f) - 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,12f,16f), uv(1f,52f,64f), 2f,
        ((1f - 0.25f) - 0.25f + 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,12f,16f), uv(0f,52f,64f), 2f,
        ((0f + 0.25f) - 0.25f - 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,12f,16f), uv(0f,52f,64f), 2f,

        //LEFT
        ((0f + 0.25f) - 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,8f,12f), uv(1f,52f,64f), 3f,
        ((0f + 0.25f) - 0.25f - 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(0f,8f,12f), uv(0f,52f,64f), 3f,
        ((0f + 0.25f) - 0.25f - 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(1f,8f,12f), uv(0f,52f,64f), 3f,
        ((0f + 0.25f) - 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,8f,12f), uv(1f,52f,64f), 3f,

        //RIGHT
        ((1f - 0.25f) - 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,0f,4f), uv(1f,52f,64f), 4f,
        ((1f - 0.25f) - 0.25f + 0.03f), (1f - 2.5f + 0.03f), (0f + 0.25f - 0.03f),                     uv(1f,0f,4f), uv(0f,52f,64f), 4f,
        ((1f - 0.25f) - 0.25f + 0.03f), (1f - 2.5f + 0.03f), (1f - 0.25f + 0.03f),                     uv(0f,0f,4f), uv(0f,52f,64f), 4f,
        ((1f - 0.25f) - 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,0f,4f), uv(1f,52f,64f), 4f,

        //BOTTOM
        ((0f + 0.25f) - 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(1f,8f,12f), uv(1f,48f,52f), 5f,
        ((1f - 0.25f) - 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (0f + 0.25f - 0.03f),              uv(0f,8f,12f), uv(1f,48f,52f), 5f,
        ((1f - 0.25f) - 0.25f + 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(0f,8f,12f), uv(0f,48f,52f), 5f,
        ((0f + 0.25f) - 0.25f - 0.03f), (0f - 2.5f - 0.5f - 0.03f), (1f - 0.25f + 0.03f),              uv(1f,8f,12f), uv(0f,48f,52f), 5f,
    )

    init {
        val head = mutableListOf<Float>()
        head.addAll(headVertices.toTypedArray())
        head.addAll(headLayerVertices.toTypedArray())

        val body = mutableListOf<Float>()
        body.addAll(bodyVertices.toTypedArray())
        body.addAll(bodyLayerVertices.toTypedArray())

        val rightArm = mutableListOf<Float>()
        rightArm.addAll(rightArmVertices.toTypedArray())
        rightArm.addAll(rightArmLayerVertices.toTypedArray())

        val leftArm = mutableListOf<Float>()
        leftArm.addAll(leftArmVertices.toTypedArray())
        leftArm.addAll(leftArmLayerVertices.toTypedArray())

        val rightLeg = mutableListOf<Float>()
        rightLeg.addAll(rightLegVertices.toTypedArray())
        rightLeg.addAll(rightLegLayerVertices.toTypedArray())

        val leftLeg = mutableListOf<Float>()
        leftLeg.addAll(leftLegVertices.toTypedArray())
        leftLeg.addAll(leftLegLayerVertices.toTypedArray())


        val parts = listOf(
            head.toFloatArray(),
            body.toFloatArray(),
            rightArm.toFloatArray(),
            leftArm.toFloatArray(),
            rightLeg.toFloatArray(),
            leftLeg.toFloatArray(),
        )

        parts.forEachIndexed { index, value ->
            stack { stack ->
                val key = vaos.keys.toList()[index]
                vaos[key] = glGenVertexArrays()

                val vboID = glGenBuffers()
                val eboID = glGenBuffers()

                glBindVertexArray(vaos[key]!!)

                // VAO
                glBindBuffer(GL_ARRAY_BUFFER, vboID)
                val vertexBuffer = stack.mallocFloat(value.size)
                vertexBuffer.put(value).flip()
                glBufferData(GL_ARRAY_BUFFER, vertexBuffer, GL_STATIC_DRAW)
                //XYZ
                glEnableVertexAttribArray(0)
                glVertexAttribPointer(0, 3, GL_FLOAT, false, 24, 0)
                //UV
                glEnableVertexAttribArray(1)
                glVertexAttribPointer(1, 2, GL_FLOAT, false, 24, 12)
                //F
                glEnableVertexAttribArray(2)
                glVertexAttribPointer(2, 1, GL_FLOAT, false, 24, 20)

                glBindBuffer(GL_ARRAY_BUFFER, 0)

                //EBO
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID)
                val indexBuffer = stack.mallocInt(indices.size)
                indexBuffer.put(indices).flip()
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, GL_STATIC_DRAW)

                glBindVertexArray(0)
            }
        }
    }
}