package dev.blackoutburst.game.entity

import dev.blackoutburst.game.utils.stack
import org.lwjgl.opengl.GL30.*
import kotlin.experimental.and

private const val LAYER = 0.05f

class PlayerModelMale(texture: Int): PlayerModel(texture) {

    override val vaos = mutableMapOf(
        "head" to 0,
        "body" to 0,
        "rightArm" to 0,
        "leftArm" to 0,
        "rightLeg" to 0,
        "leftLeg" to 0,
    )

    override val indices = mutableMapOf(
        "head" to mutableListOf<Int>(),
        "body" to mutableListOf(),
        "rightArm" to mutableListOf(),
        "leftArm" to mutableListOf(),
        "rightLeg" to mutableListOf(),
        "leftLeg" to mutableListOf(),
    )

    private fun getVoxelIndices(offset: Int) = intArrayOf(
        //TOP
        0 + offset, 2 + offset, 1 + offset,
        0 + offset, 3 + offset, 2 + offset,

        //FRONT
        4 + offset, 6 + offset, 5 + offset,
        4 + offset, 7 + offset, 6 + offset,

        //BACK
        9 + offset, 10 + offset, 8 + offset,
        10 + offset, 11 + offset, 8 + offset,

        //LEFT
        12 + offset, 14 + offset, 13 + offset,
        12 + offset, 15 + offset, 14 + offset,

        //RIGHT
        17 + offset, 18 + offset, 16 + offset,
        18 + offset, 19 + offset, 16 + offset,

        //BOTTOM
        21 + offset, 22 + offset, 20 + offset,
        22 + offset, 23 + offset, 20 + offset,
    )

    fun pixel(x: Int, y: Int): Boolean {
        var transparent = true

        stack { stack ->
            val offset = (x + y * 64) * 4
            val pixels = stack.malloc(64 * 64 * 4)
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels)

            transparent = (pixels.get(offset + 3) and 0xFF.toByte()) / 255.0f == 0f
        }

        return transparent
    }

    private fun uv(n1: Float, n2: Float, n3: Float): Float = if (n1 == 0f) n2 / 64f else n3 / 64f

    private fun voxel(x: Float, y: Float, z: Float, scale: Float, u: Float, v: Float): FloatArray {
        return floatArrayOf(
            //TOP
            (0f * scale) + x, (1f * scale) + y, (0f * scale) + z, uv(1f,u,u+1), uv(1f,v,v+1), 0f,
            (1f * scale) + x, (1f * scale) + y, (0f * scale) + z, uv(0f,u,u+1), uv(1f,v,v+1), 0f,
            (1f * scale) + x, (1f * scale) + y, (1f * scale) + z, uv(0f,u,u+1), uv(0f,v,v+1), 0f,
            (0f * scale) + x, (1f * scale) + y, (1f * scale) + z, uv(1f,u,u+1), uv(0f,v,v+1), 0f,

            //FRONT
            (0f * scale) + x, (0f * scale) + y, (0f * scale) + z, uv(1f,u,u+1), uv(1f,v,v+1), 1f,
            (1f * scale) + x, (0f * scale) + y, (0f * scale) + z, uv(0f,u,u+1), uv(1f,v,v+1), 1f,
            (1f * scale) + x, (1f * scale) + y, (0f * scale) + z, uv(0f,u,u+1), uv(0f,v,v+1), 1f,
            (0f * scale) + x, (1f * scale) + y, (0f * scale) + z, uv(1f,u,u+1), uv(0f,v,v+1), 1f,

            //BACK
            (0f * scale) + x, (0f * scale) + y, (1f * scale) + z, uv(0f,u,u+1), uv(1f,v,v+1), 2f,
            (1f * scale) + x, (0f * scale) + y, (1f * scale) + z, uv(1f,u,u+1), uv(1f,v,v+1), 2f,
            (1f * scale) + x, (1f * scale) + y, (1f * scale) + z, uv(1f,u,u+1), uv(0f,v,v+1), 2f,
            (0f * scale) + x, (1f * scale) + y, (1f * scale) + z, uv(0f,u,u+1), uv(0f,v,v+1), 2f,

            //LEFT
            (0f * scale) + x, (0f * scale) + y, (0f * scale) + z, uv(0f,u,u+1), uv(1f,v,v+1), 3f,
            (0f * scale) + x, (1f * scale) + y, (0f * scale) + z, uv(0f,u,u+1), uv(0f,v,v+1), 3f,
            (0f * scale) + x, (1f * scale) + y, (1f * scale) + z, uv(1f,u,u+1), uv(0f,v,v+1), 3f,
            (0f * scale) + x, (0f * scale) + y, (1f * scale) + z, uv(1f,u,u+1), uv(1f,v,v+1), 3f,

            //RIGHT
            (1f * scale) + x, (0f * scale) + y, (0f * scale) + z, uv(1f,u,u+1), uv(1f,v,v+1), 4f,
            (1f * scale) + x, (1f * scale) + y, (0f * scale) + z, uv(1f,u,u+1), uv(0f,v,v+1), 4f,
            (1f * scale) + x, (1f * scale) + y, (1f * scale) + z, uv(0f,u,u+1), uv(0f,v,v+1), 4f,
            (1f * scale) + x, (0f * scale) + y, (1f * scale) + z, uv(0f,u,u+1), uv(1f,v,v+1), 4f,

            //BOTTOM
            (0f * scale) + x, (0f * scale) + y, (0f * scale) + z, uv(0f,u,u+1), uv(1f,v,v+1), 5f,
            (1f * scale) + x, (0f * scale) + y, (0f * scale) + z, uv(1f,u,u+1), uv(1f,v,v+1), 5f,
            (1f * scale) + x, (0f * scale) + y, (1f * scale) + z, uv(1f,u,u+1), uv(0f,v,v+1), 5f,
            (0f * scale) + x, (0f * scale) + y, (1f * scale) + z, uv(0f,u,u+1), uv(0f,v,v+1), 5f,
        )
    }

    val partScale = -0.001f

    private val headVertices = floatArrayOf(
        //TOP
        0f - partScale, 1f + partScale, 0f - partScale, uv(1f,8f,16f), uv(1f,0f,8f), 0f,
        1f + partScale, 1f + partScale, 0f - partScale, uv(0f,8f,16f), uv(1f,0f,8f), 0f,
        1f + partScale, 1f + partScale, 1f + partScale, uv(0f,8f,16f), uv(0f,0f,8f), 0f,
        0f - partScale, 1f + partScale, 1f + partScale, uv(1f,8f,16f), uv(0f,0f,8f), 0f,

        //FRONT
        0f - partScale, 0f - partScale, 0f - partScale, uv(1f,8f,16f), uv(1f,8f,16f), 1f,
        1f + partScale, 0f - partScale, 0f - partScale, uv(0f,8f,16f), uv(1f,8f,16f), 1f,
        1f + partScale, 1f + partScale, 0f - partScale, uv(0f,8f,16f), uv(0f,8f,16f), 1f,
        0f - partScale, 1f + partScale, 0f - partScale, uv(1f,8f,16f), uv(0f,8f,16f), 1f,

        //BACK
        0f - partScale, 0f - partScale, 1f + partScale, uv(0f,24f,32f), uv(1f,8f,16f), 2f,
        1f + partScale, 0f - partScale, 1f + partScale, uv(1f,24f,32f), uv(1f,8f,16f), 2f,
        1f + partScale, 1f + partScale, 1f + partScale, uv(1f,24f,32f), uv(0f,8f,16f), 2f,
        0f - partScale, 1f + partScale, 1f + partScale, uv(0f,24f,32f), uv(0f,8f,16f), 2f,

        //LEFT
        0f - partScale, 0f - partScale, 0f - partScale, uv(0f,16f,24f), uv(1f,8f,16f), 3f,
        0f - partScale, 1f + partScale, 0f - partScale, uv(0f,16f,24f), uv(0f,8f,16f), 3f,
        0f - partScale, 1f + partScale, 1f + partScale, uv(1f,16f,24f), uv(0f,8f,16f), 3f,
        0f - partScale, 0f - partScale, 1f + partScale, uv(1f,16f,24f), uv(1f,8f,16f), 3f,

        //RIGHT
        1f + partScale, 0f - partScale, 0f - partScale, uv(1f,0f,8f), uv(1f,8f,16f), 4f,
        1f + partScale, 1f + partScale, 0f - partScale, uv(1f,0f,8f), uv(0f,8f,16f), 4f,
        1f + partScale, 1f + partScale, 1f + partScale, uv(0f,0f,8f), uv(0f,8f,16f), 4f,
        1f + partScale, 0f - partScale, 1f + partScale, uv(0f,0f,8f), uv(1f,8f,16f), 4f,

        //BOTTOM
        0f - partScale, 0f - partScale, 0f - partScale, uv(0f,16f,24f), uv(1f,0f,8f), 5f,
        1f + partScale, 0f - partScale, 0f - partScale, uv(1f,16f,24f), uv(1f,0f,8f), 5f,
        1f + partScale, 0f - partScale, 1f + partScale, uv(1f,16f,24f), uv(0f,0f,8f), 5f,
        0f - partScale, 0f - partScale, 1f + partScale, uv(0f,16f,24f), uv(0f,0f,8f), 5f,
    )

    private val headLayerVertices: FloatArray
        get() {
            val vertices = mutableListOf<Float>()
            val voxelSize = 1f/8f

            //TOP
            for (x in 40 until 48) {
                for (y in 0 until 8) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 40) * voxelSize),
                            y = 1f - voxelSize + LAYER,
                            z = (1 - ((y.toFloat() + 1) - 0) * voxelSize),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["head"]!!.addAll(getVoxelIndices(indices["head"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //FRONT
            for (x in 40 until 48) {
                for (y in 8 until 16) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 40) * voxelSize),
                            y = (1 - ((y.toFloat() + 1) - 8) * voxelSize),
                            z = 0f - LAYER,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["head"]!!.addAll(getVoxelIndices(indices["head"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BACK
            for (x in 56 until 64) {
                for (y in 8 until 16) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 56) * voxelSize),
                            y = (1 - ((y.toFloat() + 1) - 8) * voxelSize),
                            z = 1f - voxelSize + LAYER,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["head"]!!.addAll(getVoxelIndices(indices["head"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //LEFT
            for (x in 48 until 56) {
                for (y in 8 until 16) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 0f - LAYER,
                            y = (1 - ((y.toFloat() + 1) - 8) * voxelSize),
                            z = (((x.toFloat()) - 48) * voxelSize),

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["head"]!!.addAll(getVoxelIndices(indices["head"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //RIGHT
            for (x in 32 until 40) {
                for (y in 8 until 16) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 1f - voxelSize + LAYER,
                            y = (1 - ((y.toFloat() + 1) - 8) * voxelSize),
                            z = (1 - ((x.toFloat() + 1) - 32) * voxelSize),

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["head"]!!.addAll(getVoxelIndices(indices["head"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BOTTOM
            for (x in 48 until 56) {
                for (y in 0 until 8) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 48) * voxelSize),
                            y = 0f - LAYER,
                            z = (1 - ((y.toFloat() + 1) - 0) * voxelSize),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["head"]!!.addAll(getVoxelIndices(indices["head"]!!.size / 3 * 2).toTypedArray())
                }
            }

            return vertices.toFloatArray()
        }

    private val bodyVertices = floatArrayOf(
        //TOP
        (0f - partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                uv(1f,20f,28f), uv(1f,16f,20f), 0f,
        (1f + partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                uv(0f,20f,28f), uv(1f,16f,20f), 0f,
        (1f + partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                uv(0f,20f,28f), uv(0f,16f,20f), 0f,
        (0f - partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                uv(1f,20f,28f), uv(0f,16f,20f), 0f,

        //FRONT
        (0f - partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),         uv(1f,20f,28f), uv(1f,20f,32f), 1f,
        (1f + partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),         uv(0f,20f,28f), uv(1f,20f,32f), 1f,
        (1f + partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                uv(0f,20f,28f), uv(0f,20f,32f), 1f,
        (0f - partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                uv(1f,20f,28f), uv(0f,20f,32f), 1f,

        //BACK
        (0f - partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),         uv(0f,32f,40f), uv(1f,20f,32f), 2f,
        (1f + partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),         uv(1f,32f,40f), uv(1f,20f,32f), 2f,
        (1f + partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                uv(1f,32f,40f), uv(0f,20f,32f), 2f,
        (0f - partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                uv(0f,32f,40f), uv(0f,20f,32f), 2f,

        //LEFT
        (0f - partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),         uv(0f,28f,32f), uv(1f,20f,32f), 3f,
        (0f - partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                uv(0f,28f,32f), uv(0f,20f,32f), 3f,
        (0f - partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                uv(1f,28f,32f), uv(0f,20f,32f), 3f,
        (0f - partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),         uv(1f,28f,32f), uv(1f,20f,32f), 3f,

        //RIGHT
        (1f + partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),         uv(1f,16f,20f), uv(1f,20f,32f), 4f,
        (1f + partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                uv(1f,16f,20f), uv(0f,20f,32f), 4f,
        (1f + partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                uv(0f,16f,20f), uv(0f,20f,32f), 4f,
        (1f + partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),         uv(0f,16f,20f), uv(1f,20f,32f), 4f,

        //BOTTOM
        (0f - partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),         uv(0f,28f,36f), uv(1f,16f,20f), 5f,
        (1f + partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),         uv(1f,28f,36f), uv(1f,16f,20f), 5f,
        (1f + partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),         uv(1f,28f,36f), uv(0f,16f,20f), 5f,
        (0f - partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),         uv(0f,28f,36f), uv(0f,16f,20f), 5f,
    )

    private val bodyLayerVertices: FloatArray
        get() {
            val vertices = mutableListOf<Float>()
            val voxelSize = 1f/8f

            //TOP
            for (x in 20 until 28) {
                for (y in 32 until 36) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 20) * voxelSize),
                            y = 1f - voxelSize + LAYER - 1f,
                            z = (1 - ((y.toFloat() + 1) - 32) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["body"]!!.addAll(getVoxelIndices(indices["body"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //FRONT
            for (x in 20 until 28) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 20) * voxelSize),
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - 1f,
                            z = 0f - LAYER + 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["body"]!!.addAll(getVoxelIndices(indices["body"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BACK
            for (x in 32 until 40) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 32) * voxelSize),
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - 1f,
                            z = 1f - voxelSize + LAYER - 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["body"]!!.addAll(getVoxelIndices(indices["body"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //LEFT
            for (x in 28 until 32) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 0f - LAYER,
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - 1f,
                            z = (((x.toFloat()) - 28) * voxelSize) + 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["body"]!!.addAll(getVoxelIndices(indices["body"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //RIGHT
            for (x in 16 until 20) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 1f - voxelSize + LAYER,
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - 1f,
                            z = (1 - ((x.toFloat() + 1) - 16) * voxelSize) - 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["body"]!!.addAll(getVoxelIndices(indices["body"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BOTTOM
            for (x in 28 until 36) {
                for (y in 32 until 36) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 28) * voxelSize),
                            y = 0f - LAYER - 1f,
                            z = (1 - ((y.toFloat() + 1) - 32) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["body"]!!.addAll(getVoxelIndices(indices["body"]!!.size / 3 * 2).toTypedArray())
                }
            }

            return vertices.toFloatArray()
        }

    private val rightArmVertices = floatArrayOf(
        //TOP
        ((0f + 0.25f - partScale) + 0.75f), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(1f,44f,48f), uv(1f,16f,20f), 0f,
        ((1f - 0.25f + partScale) + 0.75f), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(0f,44f,48f), uv(1f,16f,20f), 0f,
        ((1f - 0.25f + partScale) + 0.75f), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(0f,44f,48f), uv(0f,16f,20f), 0f,
        ((0f + 0.25f - partScale) + 0.75f), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(1f,44f,48f), uv(0f,16f,20f), 0f,

        //FRONT
        ((0f + 0.25f) + 0.75f - partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,44f,48f), uv(1f,20f,32f), 1f,
        ((1f - 0.25f) + 0.75f + partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,44f,48f), uv(1f,20f,32f), 1f,
        ((1f - 0.25f) + 0.75f + partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(0f,44f,48f), uv(0f,20f,32f), 1f,
        ((0f + 0.25f) + 0.75f - partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(1f,44f,48f), uv(0f,20f,32f), 1f,

        //BACK
        ((0f + 0.25f) + 0.75f - partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,52f,56f), uv(1f,20f,32f), 2f,
        ((1f - 0.25f) + 0.75f + partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,52f,56f), uv(1f,20f,32f), 2f,
        ((1f - 0.25f) + 0.75f + partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(1f,52f,56f), uv(0f,20f,32f), 2f,
        ((0f + 0.25f) + 0.75f - partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(0f,52f,56f), uv(0f,20f,32f), 2f,

        //LEFT
        ((0f + 0.25f) + 0.75f - partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,48f,52f), uv(1f,20f,32f), 3f,
        ((0f + 0.25f) + 0.75f - partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(0f,48f,52f), uv(0f,20f,32f), 3f,
        ((0f + 0.25f) + 0.75f - partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(1f,48f,52f), uv(0f,20f,32f), 3f,
        ((0f + 0.25f) + 0.75f - partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,48f,52f), uv(1f,20f,32f), 3f,

        //RIGHT
        ((1f - 0.25f) + 0.75f + partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,40f,44f), uv(1f,20f,32f), 4f,
        ((1f - 0.25f) + 0.75f + partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(1f,40f,44f), uv(0f,20f,32f), 4f,
        ((1f - 0.25f) + 0.75f + partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(0f,40f,44f), uv(0f,20f,32f), 4f,
        ((1f - 0.25f) + 0.75f + partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,40f,44f), uv(1f,20f,32f), 4f,

        //BOTTOM
        ((0f + 0.25f) + 0.75f - partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,48f,52f), uv(1f,16f,20f), 5f,
        ((1f - 0.25f) + 0.75f + partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,48f,52f), uv(1f,16f,20f), 5f,
        ((1f - 0.25f) + 0.75f + partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,48f,52f), uv(0f,16f,20f), 5f,
        ((0f + 0.25f) + 0.75f - partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,48f,52f), uv(0f,16f,20f), 5f,
    )

    private val rightArmLayerVertices: FloatArray
        get() {
            val vertices = mutableListOf<Float>()
            val voxelSize = 1f/8f

            //TOP
            for (x in 44 until 48) {
                for (y in 32 until 36) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 44) * voxelSize) + (voxelSize * 4),
                            y = 1f - voxelSize + LAYER - 1f,
                            z = (1 - ((y.toFloat() + 1) - 32) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightArm"]!!.addAll(getVoxelIndices(indices["rightArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //FRONT
            for (x in 44 until 48) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 44) * voxelSize) + (voxelSize * 4),
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - 1f,
                            z = 0f - LAYER + 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightArm"]!!.addAll(getVoxelIndices(indices["rightArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BACK
            for (x in 52 until 56) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 52) * voxelSize) + (voxelSize * 4),
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - 1f,
                            z = 1f - voxelSize + LAYER - 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightArm"]!!.addAll(getVoxelIndices(indices["rightArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //LEFT
            for (x in 48 until 52) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 0f - LAYER + (voxelSize * 8),
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - 1f,
                            z = (((x.toFloat()) - 48) * voxelSize) + 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightArm"]!!.addAll(getVoxelIndices(indices["rightArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //RIGHT
            for (x in 40 until 44) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 1f - voxelSize + LAYER + (voxelSize * 4),
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - 1f,
                            z = (1 - ((x.toFloat() + 1) - 40) * voxelSize) - 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightArm"]!!.addAll(getVoxelIndices(indices["rightArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BOTTOM
            for (x in 48 until 52) {
                for (y in 32 until 36) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 48) * voxelSize) + (voxelSize * 4),
                            y = 0f - LAYER - 1f - (voxelSize * 4),
                            z = (1 - ((y.toFloat() + 1) - 32) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightArm"]!!.addAll(getVoxelIndices(indices["rightArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            return vertices.toFloatArray()
        }



    private val leftArmVertices = floatArrayOf(
        //TOP
        ((0f + 0.25f) - 0.75f - partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(1f,36f,40f), uv(1f,48f,52f), 0f,
        ((1f - 0.25f) - 0.75f + partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(0f,36f,40f), uv(1f,48f,52f), 0f,
        ((1f - 0.25f) - 0.75f + partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(0f,36f,40f), uv(0f,48f,52f), 0f,
        ((0f + 0.25f) - 0.75f - partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(1f,36f,40f), uv(0f,48f,52f), 0f,

        //FRONT
        ((0f + 0.25f) - 0.75f - partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,36f,40f), uv(1f,52f,64f), 1f,
        ((1f - 0.25f) - 0.75f + partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,36f,40f), uv(1f,52f,64f), 1f,
        ((1f - 0.25f) - 0.75f + partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(0f,36f,40f), uv(0f,52f,64f), 1f,
        ((0f + 0.25f) - 0.75f - partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(1f,36f,40f), uv(0f,52f,64f), 1f,

        //BACK
        ((0f + 0.25f) - 0.75f - partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,44f,48f), uv(1f,52f,64f), 2f,
        ((1f - 0.25f) - 0.75f + partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,44f,48f), uv(1f,52f,64f), 2f,
        ((1f - 0.25f) - 0.75f + partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(1f,44f,48f), uv(0f,52f,64f), 2f,
        ((0f + 0.25f) - 0.75f - partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(0f,44f,48f), uv(0f,52f,64f), 2f,

        //LEFT
        ((0f + 0.25f) - 0.75f - partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,40f,44f), uv(1f,52f,64f), 3f,
        ((0f + 0.25f) - 0.75f - partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(0f,40f,44f), uv(0f,52f,64f), 3f,
        ((0f + 0.25f) - 0.75f - partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(1f,40f,44f), uv(0f,52f,64f), 3f,
        ((0f + 0.25f) - 0.75f - partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,40f,44f), uv(1f,52f,64f), 3f,

        //RIGHT
        ((1f - 0.25f) - 0.75f + partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,32f,36f), uv(1f,52f,64f), 4f,
        ((1f - 0.25f) - 0.75f + partScale), (1f - 1f + partScale), (0f + 0.25f - partScale),                     uv(1f,32f,36f), uv(0f,52f,64f), 4f,
        ((1f - 0.25f) - 0.75f + partScale), (1f - 1f + partScale), (1f - 0.25f + partScale),                     uv(0f,32f,36f), uv(0f,52f,64f), 4f,
        ((1f - 0.25f) - 0.75f + partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,32f,36f), uv(1f,52f,64f), 4f,

        //BOTTOM
        ((0f + 0.25f) - 0.75f - partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,40f,44f), uv(1f,48f,52f), 5f,
        ((1f - 0.25f) - 0.75f + partScale), (0f - 1f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,40f,44f), uv(1f,48f,52f), 5f,
        ((1f - 0.25f) - 0.75f + partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,40f,44f), uv(0f,48f,52f), 5f,
        ((0f + 0.25f) - 0.75f - partScale), (0f - 1f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,40f,44f), uv(0f,48f,52f), 5f,
    )

    private val leftArmLayerVertices: FloatArray
        get()   {
            val vertices = mutableListOf<Float>()
            val voxelSize = 1f/8f

            //TOP
            for (x in 52 until 56) {
                for (y in 48 until 52) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 52) * voxelSize) - (voxelSize * 8),
                            y = 1f - voxelSize + LAYER - 1f,
                            z = (1 - ((y.toFloat() + 1) - 48) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftArm"]!!.addAll(getVoxelIndices(indices["leftArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //FRONT
            for (x in 52 until 56) {
                for (y in 52 until 64) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 52) * voxelSize) - (voxelSize * 8),
                            y = (1 - ((y.toFloat() + 1) - 52f) * voxelSize) - 1f,
                            z = 0f - LAYER + 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftArm"]!!.addAll(getVoxelIndices(indices["leftArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BACK
            for (x in 60 until 64) {
                for (y in 52 until 64) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 60) * voxelSize) - (voxelSize * 8),
                            y = (1 - ((y.toFloat() + 1) - 52) * voxelSize) - 1f,
                            z = 1f - voxelSize + LAYER - 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftArm"]!!.addAll(getVoxelIndices(indices["leftArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //LEFT
            for (x in 56 until 60) {
                for (y in 52 until 64) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 0f - LAYER - (voxelSize * 4),
                            y = (1 - ((y.toFloat() + 1) - 52) * voxelSize) - 1f,
                            z = (((x.toFloat()) - 56) * voxelSize) + 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftArm"]!!.addAll(getVoxelIndices(indices["leftArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //RIGHT
            for (x in 48 until 52) {
                for (y in 52 until 64) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 1f - voxelSize + LAYER - (voxelSize * 8),
                            y = (1 - ((y.toFloat() + 1) - 52) * voxelSize) - 1f,
                            z = (1 - ((x.toFloat() + 1) - 48) * voxelSize) - 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftArm"]!!.addAll(getVoxelIndices(indices["leftArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BOTTOM
            for (x in 56 until 60) {
                for (y in 48 until 52) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 56) * voxelSize) - (voxelSize * 8),
                            y = 0f - LAYER - 1f - (voxelSize * 4),
                            z = (1 - ((y.toFloat() + 1) - 48) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftArm"]!!.addAll(getVoxelIndices(indices["leftArm"]!!.size / 3 * 2).toTypedArray())
                }
            }

            return vertices.toFloatArray()
        }

    private val rightLegVertices = floatArrayOf(
        //TOP
        ((0f + 0.25f) + 0.25f - partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(1f,4f,8f), uv(1f,16f,20f), 0f,
        ((1f - 0.25f) + 0.25f + partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(0f,4f,8f), uv(1f,16f,20f), 0f,
        ((1f - 0.25f) + 0.25f + partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(0f,4f,8f), uv(0f,16f,20f), 0f,
        ((0f + 0.25f) + 0.25f - partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(1f,4f,8f), uv(0f,16f,20f), 0f,

        //FRONT
        ((0f + 0.25f) + 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,4f,8f), uv(1f,20f,32f), 1f,
        ((1f - 0.25f) + 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,4f,8f), uv(1f,20f,32f), 1f,
        ((1f - 0.25f) + 0.25f + partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(0f,4f,8f), uv(0f,20f,32f), 1f,
        ((0f + 0.25f) + 0.25f - partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(1f,4f,8f), uv(0f,20f,32f), 1f,

        //BACK
        ((0f + 0.25f) + 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,12f,16f), uv(1f,20f,32f), 2f,
        ((1f - 0.25f) + 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,12f,16f), uv(1f,20f,32f), 2f,
        ((1f - 0.25f) + 0.25f + partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(1f,12f,16f), uv(0f,20f,32f), 2f,
        ((0f + 0.25f) + 0.25f - partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(0f,12f,16f), uv(0f,20f,32f), 2f,

        //LEFT
        ((0f + 0.25f) + 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,8f,12f), uv(1f,20f,32f), 3f,
        ((0f + 0.25f) + 0.25f - partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(0f,8f,12f), uv(0f,20f,32f), 3f,
        ((0f + 0.25f) + 0.25f - partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(1f,8f,12f), uv(0f,20f,32f), 3f,
        ((0f + 0.25f) + 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,8f,12f), uv(1f,20f,32f), 3f,

        //RIGHT
        ((1f - 0.25f) + 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,0f,4f), uv(1f,20f,32f), 4f,
        ((1f - 0.25f) + 0.25f + partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(1f,0f,4f), uv(0f,20f,32f), 4f,
        ((1f - 0.25f) + 0.25f + partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(0f,0f,4f), uv(0f,20f,32f), 4f,
        ((1f - 0.25f) + 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,0f,4f), uv(1f,20f,32f), 4f,

        //BOTTOM
        ((0f + 0.25f) + 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,8f,12f), uv(1f,16f,20f), 5f,
        ((1f - 0.25f) + 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,8f,12f), uv(1f,16f,20f), 5f,
        ((1f - 0.25f) + 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,8f,12f), uv(0f,16f,20f), 5f,
        ((0f + 0.25f) + 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,8f,12f), uv(0f,16f,20f), 5f,
    )

    private val rightLegLayerVertices: FloatArray
        get()   {
            val vertices = mutableListOf<Float>()
            val voxelSize = 1f/8f

            //TOP
            for (x in 4 until 8) {
                for (y in 32 until 36) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 4) * voxelSize),
                            y = 1f - voxelSize + LAYER  - (voxelSize * 20),
                            z = (1 - ((y.toFloat() + 1) - 32) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightLeg"]!!.addAll(getVoxelIndices(indices["rightLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //FRONT
            for (x in 4 until 8) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 4) * voxelSize),
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - (voxelSize * 20),
                            z = 0f - LAYER + 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightLeg"]!!.addAll(getVoxelIndices(indices["rightLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BACK
            for (x in 12 until 16) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 12) * voxelSize),
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - (voxelSize * 20),
                            z = 1f - voxelSize + LAYER - 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightLeg"]!!.addAll(getVoxelIndices(indices["rightLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //LEFT
            for (x in 8 until 12) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 0f - LAYER + (voxelSize * 4),
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - (voxelSize * 20),
                            z = (((x.toFloat()) - 8) * voxelSize) + 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightLeg"]!!.addAll(getVoxelIndices(indices["rightLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //RIGHT
            for (x in 0 until 4) {
                for (y in 36 until 48) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 1f - voxelSize + LAYER,
                            y = (1 - ((y.toFloat() + 1) - 36) * voxelSize) - (voxelSize * 20),
                            z = (1 - ((x.toFloat() + 1) - 0) * voxelSize) - 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightLeg"]!!.addAll(getVoxelIndices(indices["rightLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BOTTOM
            for (x in 8 until 12) {
                for (y in 32 until 36) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 8) * voxelSize),
                            y = 0f - LAYER  - (voxelSize * 20) - (voxelSize * 4),
                            z = (1 - ((y.toFloat() + 1) - 32) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["rightLeg"]!!.addAll(getVoxelIndices(indices["rightLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            return vertices.toFloatArray()
        }

    private val leftLegVertices = floatArrayOf(
        //TOP
        ((0f + 0.25f) - 0.25f - partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(1f,20f,24f), uv(1f,48f,52f), 0f,
        ((1f - 0.25f) - 0.25f + partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(0f,20f,24f), uv(1f,48f,52f), 0f,
        ((1f - 0.25f) - 0.25f + partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(0f,20f,24f), uv(0f,48f,52f), 0f,
        ((0f + 0.25f) - 0.25f - partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(1f,20f,24f), uv(0f,48f,52f), 0f,

        //FRONT
        ((0f + 0.25f) - 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,20f,24f), uv(1f,52f,64f), 1f,
        ((1f - 0.25f) - 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,20f,24f), uv(1f,52f,64f), 1f,
        ((1f - 0.25f) - 0.25f + partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(0f,20f,24f), uv(0f,52f,64f), 1f,
        ((0f + 0.25f) - 0.25f - partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(1f,20f,24f), uv(0f,52f,64f), 1f,

        //BACK
        ((0f + 0.25f) - 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,28f,32f), uv(1f,52f,64f), 2f,
        ((1f - 0.25f) - 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,28f,32f), uv(1f,52f,64f), 2f,
        ((1f - 0.25f) - 0.25f + partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(1f,28f,32f), uv(0f,52f,64f), 2f,
        ((0f + 0.25f) - 0.25f - partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(0f,28f,32f), uv(0f,52f,64f), 2f,

        //LEFT
        ((0f + 0.25f) - 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,24f,28f), uv(1f,52f,64f), 3f,
        ((0f + 0.25f) - 0.25f - partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(0f,24f,28f), uv(0f,52f,64f), 3f,
        ((0f + 0.25f) - 0.25f - partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(1f,24f,28f), uv(0f,52f,64f), 3f,
        ((0f + 0.25f) - 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,24f,28f), uv(1f,52f,64f), 3f,

        //RIGHT
        ((1f - 0.25f) - 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,16f,20f), uv(1f,52f,64f), 4f,
        ((1f - 0.25f) - 0.25f + partScale), (1f - 2.5f + partScale), (0f + 0.25f - partScale),                     uv(1f,16f,20f), uv(0f,52f,64f), 4f,
        ((1f - 0.25f) - 0.25f + partScale), (1f - 2.5f + partScale), (1f - 0.25f + partScale),                     uv(0f,16f,20f), uv(0f,52f,64f), 4f,
        ((1f - 0.25f) - 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,16f,20f), uv(1f,52f,64f), 4f,

        //BOTTOM
        ((0f + 0.25f) - 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(1f,24f,28f), uv(1f,48f,52f), 5f,
        ((1f - 0.25f) - 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (0f + 0.25f - partScale),              uv(0f,24f,28f), uv(1f,48f,52f), 5f,
        ((1f - 0.25f) - 0.25f + partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(0f,24f,28f), uv(0f,48f,52f), 5f,
        ((0f + 0.25f) - 0.25f - partScale), (0f - 2.5f - 0.5f - partScale), (1f - 0.25f + partScale),              uv(1f,24f,28f), uv(0f,48f,52f), 5f,
    )

    private val leftLegLayerVertices: FloatArray
        get()   {
            val vertices = mutableListOf<Float>()
            val voxelSize = 1f/8f

            //TOP
            for (x in 4 until 8) {
                for (y in 48 until 52) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 4) * voxelSize) - (voxelSize * 4),
                            y = 1f - voxelSize + LAYER  - (voxelSize * 20),
                            z = (1 - ((y.toFloat() + 1) - 48) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftLeg"]!!.addAll(getVoxelIndices(indices["leftLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //FRONT
            for (x in 4 until 8) {
                for (y in 52 until 64) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 4) * voxelSize) - (voxelSize * 4),
                            y = (1 - ((y.toFloat() + 1) - 52) * voxelSize) - (voxelSize * 20),
                            z = 0f - LAYER + 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftLeg"]!!.addAll(getVoxelIndices(indices["leftLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BACK
            for (x in 12 until 16) {
                for (y in 52 until 64) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 12) * voxelSize) - (voxelSize * 4),
                            y = (1 - ((y.toFloat() + 1) - 52) * voxelSize) - (voxelSize * 20),
                            z = 1f - voxelSize + LAYER - 2f/8f,
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftLeg"]!!.addAll(getVoxelIndices(indices["leftLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //LEFT
            for (x in 8 until 12) {
                for (y in 52 until 64) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 0f - LAYER + (voxelSize * 4) - (voxelSize * 4),
                            y = (1 - ((y.toFloat() + 1) - 52) * voxelSize) - (voxelSize * 20),
                            z = (((x.toFloat()) - 8) * voxelSize) + 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftLeg"]!!.addAll(getVoxelIndices(indices["leftLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //RIGHT
            for (x in 0 until 4) {
                for (y in 52 until 64) {
                    val transparent = pixel(x, y)
                    if (transparent) continue
                    vertices.addAll(
                        voxel(
                            x = 1f - voxelSize + LAYER - (voxelSize * 4),
                            y = (1 - ((y.toFloat() + 1) - 52) * voxelSize) - (voxelSize * 20),
                            z = (1 - ((x.toFloat() + 1) - 0) * voxelSize) - 2f/8f,

                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftLeg"]!!.addAll(getVoxelIndices(indices["leftLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            //BOTTOM
            for (x in 8 until 12) {
                for (y in 48 until 52) {
                    val transparent = pixel(x, y)
                    if (transparent) continue

                    vertices.addAll(
                        voxel(
                            x = (1 - ((x.toFloat() + 1) - 8) * voxelSize) - (voxelSize * 4),
                            y = 0f - LAYER  - (voxelSize * 20) - (voxelSize * 4),
                            z = (1 - ((y.toFloat() + 1) - 48) * voxelSize) - (voxelSize * 2),
                            scale = voxelSize,
                            u = x.toFloat(),
                            v = y.toFloat()
                        ).toTypedArray()
                    )
                    indices["leftLeg"]!!.addAll(getVoxelIndices(indices["leftLeg"]!!.size / 3 * 2).toTypedArray())
                }
            }

            return vertices.toFloatArray()
        }

    init {
        glBindTexture(GL_TEXTURE_2D, texture)

        val head = mutableListOf<Float>()
        head.addAll(headVertices.toTypedArray())
        indices["head"]!!.addAll(getVoxelIndices(0).toTypedArray())
        head.addAll(headLayerVertices.toTypedArray())

        val body = mutableListOf<Float>()
        body.addAll(bodyVertices.toTypedArray())
        indices["body"]!!.addAll(getVoxelIndices(0).toTypedArray())
        body.addAll(bodyLayerVertices.toTypedArray())

        val rightArm = mutableListOf<Float>()
        rightArm.addAll(rightArmVertices.toTypedArray())
        indices["rightArm"]!!.addAll(getVoxelIndices(0).toTypedArray())
        rightArm.addAll(rightArmLayerVertices.toTypedArray())

        val leftArm = mutableListOf<Float>()
        leftArm.addAll(leftArmVertices.toTypedArray())
        indices["leftArm"]!!.addAll(getVoxelIndices(0).toTypedArray())
        leftArm.addAll(leftArmLayerVertices.toTypedArray())

        val rightLeg = mutableListOf<Float>()
        rightLeg.addAll(rightLegVertices.toTypedArray())
        indices["rightLeg"]!!.addAll(getVoxelIndices(0).toTypedArray())
        rightLeg.addAll(rightLegLayerVertices.toTypedArray())

        val leftLeg = mutableListOf<Float>()
        leftLeg.addAll(leftLegVertices.toTypedArray())
        indices["leftLeg"]!!.addAll(getVoxelIndices(0).toTypedArray())
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
            stack(1024 * 1024) { stack ->
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
                val indexBuffer = stack.mallocInt(indices[key]!!.size)
                indexBuffer.put(indices[key]!!.toIntArray()).flip()
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, GL_STATIC_DRAW)

                glBindVertexArray(0)
            }
        }
    }
}