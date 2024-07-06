package dev.blackoutburst.game.camera

import dev.blackoutburst.game.maths.Matrix
import dev.blackoutburst.game.maths.Vector3i
import dev.blackoutburst.game.maths.Vector4f
import dev.blackoutburst.game.world.Chunk

object FrustumCulling {
    val planes = Array(6) { Vector4f() }

    fun update(projMatrix: Matrix, viewMatrix: Matrix) {
        val projCopy = Matrix(projMatrix)
        val viewCopy = Matrix(viewMatrix)
        val combinedMatrix = projCopy.mul(viewCopy)

        for (i in 0 until 6) {
            planes[i].set(
                combinedMatrix.m03 + combinedMatrix.get(i / 2) * if (i % 2 == 0) 1 else -1,
                combinedMatrix.m13 + combinedMatrix.get(4 + (i / 2) % 3) * if (i % 2 == 0) 1 else -1,
                combinedMatrix.m23 + combinedMatrix.get(8 + (i / 2) % 3) * if (i % 2 == 0) 1 else -1,
                combinedMatrix.m33 + combinedMatrix.get(12 + (i / 2) % 3) * if (i % 2 == 0) 1 else -1
            ).normalize()
        }
    }

    fun isInFrustum(chunk: Chunk): Boolean {
        val minCorner = chunk.position
        val maxCorner = chunk.position + Vector3i(16)

        for (plane in planes) {
            if (plane.x * minCorner.x + plane.y * minCorner.y + plane.z * minCorner.z + plane.w > 0) continue
            if (plane.x * maxCorner.x + plane.y * minCorner.y + plane.z * minCorner.z + plane.w > 0) continue
            if (plane.x * minCorner.x + plane.y * maxCorner.y + plane.z * minCorner.z + plane.w > 0) continue
            if (plane.x * maxCorner.x + plane.y * maxCorner.y + plane.z * minCorner.z + plane.w > 0) continue
            if (plane.x * minCorner.x + plane.y * minCorner.y + plane.z * maxCorner.z + plane.w > 0) continue
            if (plane.x * maxCorner.x + plane.y * minCorner.y + plane.z * maxCorner.z + plane.w > 0) continue
            if (plane.x * minCorner.x + plane.y * maxCorner.y + plane.z * maxCorner.z + plane.w > 0) continue
            if (plane.x * maxCorner.x + plane.y * maxCorner.y + plane.z * maxCorner.z + plane.w > 0) continue
            return false
        }
        return true
    }
}