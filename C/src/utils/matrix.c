#include <stdlib.h>
#include "utils/math.h"
#include "utils/matrix.h"
#include "utils/types.h"

void matrixSetIdentity(MATRIX* matrix) {
    matrix->m00 = 1.0f;
    matrix->m01 = 0.0f;
    matrix->m02 = 0.0f;
    matrix->m03 = 0.0f;

    matrix->m10 = 0.0f;
    matrix->m11 = 1.0f;
    matrix->m12 = 0.0f;
    matrix->m13 = 0.0f;

    matrix->m20 = 0.0f;
    matrix->m21 = 0.0f;
    matrix->m22 = 1.0f;
    matrix->m23 = 0.0f;

    matrix->m30 = 0.0f;
    matrix->m31 = 0.0f;
    matrix->m32 = 0.0f;
    matrix->m33 = 1.0f;
}

F32* matrixGetValues(MATRIX* matrix) {
    F32* values = malloc(sizeof(F32) * 16);
    values[M00] = matrix->m00;
    values[M01] = matrix->m01;
    values[M02] = matrix->m02;
    values[M03] = matrix->m03;
    
    values[M10] = matrix->m10;
    values[M11] = matrix->m11;
    values[M12] = matrix->m12;
    values[M13] = matrix->m13;

    values[M20] = matrix->m20;
    values[M21] = matrix->m21;
    values[M22] = matrix->m22;
    values[M23] = matrix->m23;

    values[M30] = matrix->m30;
    values[M31] = matrix->m31;
    values[M32] = matrix->m32;
    values[M33] = matrix->m33;

    return values;
}

MATRIX* identityMatrix() {
    MATRIX* matrix = malloc(sizeof(MATRIX));
    matrixSetIdentity(matrix);

    return matrix;
}

F32 matrixGet(MATRIX* matrix, U32 index) {
    F32 values[] = {
        matrix->m00, matrix->m01, matrix->m02, matrix->m03,
        matrix->m10, matrix->m11, matrix->m12, matrix->m13,
        matrix->m20, matrix->m21, matrix->m22, matrix->m23,
        matrix->m30, matrix->m31, matrix->m32, matrix->m33,
    };

    return values[index];
}

void matrixMultiply(MATRIX* result, MATRIX* left, MATRIX* right) {
    result->m00 = left->m00 * right->m00 + left->m01 * right->m10 + left->m02 * right->m20 + left->m03 * right->m30;
    result->m01 = left->m00 * right->m01 + left->m01 * right->m11 + left->m02 * right->m21 + left->m03 * right->m31;
    result->m02 = left->m00 * right->m02 + left->m01 * right->m12 + left->m02 * right->m22 + left->m03 * right->m32;
    result->m03 = left->m00 * right->m03 + left->m01 * right->m13 + left->m02 * right->m23 + left->m03 * right->m33;

    result->m10 = left->m10 * right->m00 + left->m11 * right->m10 + left->m12 * right->m20 + left->m13 * right->m30;
    result->m11 = left->m10 * right->m01 + left->m11 * right->m11 + left->m12 * right->m21 + left->m13 * right->m31;
    result->m12 = left->m10 * right->m02 + left->m11 * right->m12 + left->m12 * right->m22 + left->m13 * right->m32;
    result->m13 = left->m10 * right->m03 + left->m11 * right->m13 + left->m12 * right->m23 + left->m13 * right->m33;

    result->m20 = left->m20 * right->m00 + left->m21 * right->m10 + left->m22 * right->m20 + left->m23 * right->m30;
    result->m21 = left->m20 * right->m01 + left->m21 * right->m11 + left->m22 * right->m21 + left->m23 * right->m31;
    result->m22 = left->m20 * right->m02 + left->m21 * right->m12 + left->m22 * right->m22 + left->m23 * right->m32;
    result->m23 = left->m20 * right->m03 + left->m21 * right->m13 + left->m22 * right->m23 + left->m23 * right->m33;

    result->m30 = left->m30 * right->m00 + left->m31 * right->m10 + left->m32 * right->m20 + left->m33 * right->m30;
    result->m31 = left->m30 * right->m01 + left->m31 * right->m11 + left->m32 * right->m21 + left->m33 * right->m31;
    result->m32 = left->m30 * right->m02 + left->m31 * right->m12 + left->m32 * right->m22 + left->m33 * right->m32;
    result->m33 = left->m30 * right->m03 + left->m31 * right->m13 + left->m32 * right->m23 + left->m33 * right->m33;
}


void matrixCopy(MATRIX* src, MATRIX* dest) {
    dest->m00 = src->m00;
    dest->m01 = src->m01;
    dest->m02 = src->m02;
    dest->m03 = src->m03;

    dest->m10 = src->m10;
    dest->m11 = src->m11;
    dest->m12 = src->m12;
    dest->m13 = src->m13;

    dest->m20 = src->m20;
    dest->m21 = src->m21;
    dest->m22 = src->m22;
    dest->m23 = src->m23;

    dest->m30 = src->m30;
    dest->m31 = src->m31;
    dest->m32 = src->m32;
    dest->m33 = src->m33;
}

void matrixOrtho2D(MATRIX* matrix, F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far) {
    F32 xOrt = 2.0f / (right - left);
    F32 yOrt = 2.0f / (top - bottom);
    F32 zOrt = -2.0f / (far - near);

    F32 tx = -(right + left) / (right - left);
    F32 ty = -(top + bottom) / (top - bottom);
    F32 tz = -(far + near) / (far - near);

    matrix->m00 = xOrt;
    matrix->m10 = 0.0f;
    matrix->m20 = 0.0f;
    matrix->m30 = tx;

    matrix->m01 = 0.0f;
    matrix->m11 = yOrt;
    matrix->m21 = 0.0f;
    matrix->m31 = ty;

    matrix->m02 = 0.0f;
    matrix->m12 = 0.0f;
    matrix->m22 = zOrt;
    matrix->m32 = tz;

    matrix->m03 = 0.0f;
    matrix->m13 = 0.0f;
    matrix->m23 = 0.0f;
    matrix->m33 = 1.0f;
}

void matrixProjection(MATRIX* matrix, F32 width, F32 height, F32 fov, F32 near, F32 far) {
    F32 aspectRatio = width / height;
    F32 yScale = 1.0f / tan(rad(fov / 2.0f));
    F32 xScale = yScale / aspectRatio;
    F32 frustumLength = far - near;

    matrix->m00 = xScale;
    matrix->m11 = yScale;
    matrix->m22 = -((far + near) / frustumLength);
    matrix->m23 = -1.0f;
    matrix->m32 = -((2.0f * near * far) / frustumLength);
    matrix->m33 = 0.0f;
}

void matrixScale2d(MATRIX* matrix, F32 x, F32 y) {
    matrix->m00 = matrix->m00 * x;
    matrix->m01 = matrix->m01 * x;
    matrix->m02 = matrix->m02 * x;
    matrix->m03 = matrix->m03 * x;
    matrix->m10 = matrix->m10 * y;
    matrix->m11 = matrix->m11 * y;
    matrix->m12 = matrix->m12 * y;
    matrix->m13 = matrix->m13 * y;
}

void matrixScale2dP(MATRIX* matrix, VECTORF* vector) {
    matrix->m00 = matrix->m00 * vector->x;
    matrix->m01 = matrix->m01 * vector->x;
    matrix->m02 = matrix->m02 * vector->x;
    matrix->m03 = matrix->m03 * vector->x;
    matrix->m10 = matrix->m10 * vector->y;
    matrix->m11 = matrix->m11 * vector->y;
    matrix->m12 = matrix->m12 * vector->y;
    matrix->m13 = matrix->m13 * vector->y;
}

void matrixScale3d(MATRIX* matrix, F32 x, F32 y, F32 z) {
    matrix->m00 = matrix->m00 * x;
    matrix->m01 = matrix->m01 * x;
    matrix->m02 = matrix->m02 * x;
    matrix->m03 = matrix->m03 * x;
    matrix->m10 = matrix->m10 * y;
    matrix->m11 = matrix->m11 * y;
    matrix->m12 = matrix->m12 * y;
    matrix->m13 = matrix->m13 * y;
    matrix->m20 = matrix->m20 * z;
    matrix->m21 = matrix->m21 * z;
    matrix->m22 = matrix->m22 * z;
    matrix->m23 = matrix->m23 * z;
}

void matrixScale3dP(MATRIX* matrix, VECTORF* vector) {
    matrix->m00 = matrix->m00 * vector->x;
    matrix->m01 = matrix->m01 * vector->x;
    matrix->m02 = matrix->m02 * vector->x;
    matrix->m03 = matrix->m03 * vector->x;
    matrix->m10 = matrix->m10 * vector->y;
    matrix->m11 = matrix->m11 * vector->y;
    matrix->m12 = matrix->m12 * vector->y;
    matrix->m13 = matrix->m13 * vector->y;
    matrix->m20 = matrix->m20 * vector->z;
    matrix->m21 = matrix->m21 * vector->z;
    matrix->m22 = matrix->m22 * vector->z;
    matrix->m23 = matrix->m23 * vector->z;
}

void matrixTranslate2d(MATRIX* matrix, F32 x, F32 y) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    matrix->m30 += src->m00 * x + src->m10 * y;
    matrix->m31 += src->m01 * x + src->m11 * y;
    matrix->m32 += src->m02 * x + src->m12 * y;
    matrix->m33 += src->m03 * x + src->m13 * y;

    free(src);
}

void matrixTranslate2dP(MATRIX* matrix, VECTORF* vector) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    matrix->m30 += src->m00 * vector->x + src->m10 * vector->y;
    matrix->m31 += src->m01 * vector->x + src->m11 * vector->y;
    matrix->m32 += src->m02 * vector->x + src->m12 * vector->y;
    matrix->m33 += src->m03 * vector->x + src->m13 * vector->y;

    free(src);
}

void matrixTranslate3d(MATRIX* matrix, F32 x, F32 y, F32 z) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    matrix->m30 += src->m00 * x + src->m10 * y + src->m20 * z;
    matrix->m31 += src->m01 * x + src->m11 * y + src->m21 * z;
    matrix->m32 += src->m02 * x + src->m12 * y + src->m22 * z;
    matrix->m33 += src->m03 * x + src->m13 * y + src->m23 * z;

    free(src);
}

void matrixTranslate3dP(MATRIX* matrix, VECTORF* vector) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    matrix->m30 += src->m00 * vector->x + src->m10 * vector->y + src->m20 * vector->z;
    matrix->m31 += src->m01 * vector->x + src->m11 * vector->y + src->m21 * vector->z;
    matrix->m32 += src->m02 * vector->x + src->m12 * vector->y + src->m22 * vector->z;
    matrix->m33 += src->m03 * vector->x + src->m13 * vector->y + src->m23 * vector->z;

    free(src);
}

void matrixRotateP(MATRIX* matrix, F32 angle, VECTORF* vector) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    F32 c = cos(angle);
    F32 s = sin(angle);
    F32 oneMinusC = 1.0f - c;
    F32 xy = vector->x * vector->y;
    F32 yz = vector->y * vector->z;
    F32 xz = vector->x * vector->z;
    F32 xs = vector->x * s;
    F32 ys = vector->y * s;
    F32 zs = vector->z * s;

    F32 f00 = vector->x * vector->x * oneMinusC + c;
    F32 f01 = xy * oneMinusC + zs;
    F32 f02 = xz * oneMinusC - ys;
    F32 f10 = xy * oneMinusC - zs;
    F32 f11 = vector->y * vector->y * oneMinusC + c;
    F32 f12 = yz * oneMinusC + xs;
    F32 f20 = xz * oneMinusC + ys;
    F32 f21 = yz * oneMinusC - xs;
    F32 f22 = vector->z * vector->z * oneMinusC + c;

    F32 t00 = src->m00 * f00 + src->m10 * f01 + src->m20 * f02;
    F32 t01 = src->m01 * f00 + src->m11 * f01 + src->m21 * f02;
    F32 t02 = src->m02 * f00 + src->m12 * f01 + src->m22 * f02;
    F32 t03 = src->m03 * f00 + src->m13 * f01 + src->m23 * f02;
    F32 t10 = src->m00 * f10 + src->m10 * f11 + src->m20 * f12;
    F32 t11 = src->m01 * f10 + src->m11 * f11 + src->m21 * f12;
    F32 t12 = src->m02 * f10 + src->m12 * f11 + src->m22 * f12;
    F32 t13 = src->m03 * f10 + src->m13 * f11 + src->m23 * f12;

    matrix->m20 = src->m00 * f20 + src->m10 * f21 + src->m20 * f22;
    matrix->m21 = src->m01 * f20 + src->m11 * f21 + src->m21 * f22;
    matrix->m22 = src->m02 * f20 + src->m12 * f21 + src->m22 * f22;
    matrix->m23 = src->m03 * f20 + src->m13 * f21 + src->m23 * f22;
    matrix->m00 = t00;
    matrix->m01 = t01;
    matrix->m02 = t02;
    matrix->m03 = t03;
    matrix->m10 = t10;
    matrix->m11 = t11;
    matrix->m12 = t12;
    matrix->m13 = t13;

    free(src);
}

void matrixRotate(MATRIX* matrix, F32 angle, F32 x, F32 y, F32 z) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    F32 c = cos(angle);
    F32 s = sin(angle);
    F32 oneMinusC = 1.0f - c;
    F32 xy = x * y;
    F32 yz = y * z;
    F32 xz = x * z;
    F32 xs = x * s;
    F32 ys = y * s;
    F32 zs = z * s;

    F32 f00 = x * x * oneMinusC + c;
    F32 f01 = xy * oneMinusC + zs;
    F32 f02 = xz * oneMinusC - ys;
    F32 f10 = xy * oneMinusC - zs;
    F32 f11 = y * y * oneMinusC + c;
    F32 f12 = yz * oneMinusC + xs;
    F32 f20 = xz * oneMinusC + ys;
    F32 f21 = yz * oneMinusC - xs;
    F32 f22 = z * z * oneMinusC + c;

    F32 t00 = src->m00 * f00 + src->m10 * f01 + src->m20 * f02;
    F32 t01 = src->m01 * f00 + src->m11 * f01 + src->m21 * f02;
    F32 t02 = src->m02 * f00 + src->m12 * f01 + src->m22 * f02;
    F32 t03 = src->m03 * f00 + src->m13 * f01 + src->m23 * f02;
    F32 t10 = src->m00 * f10 + src->m10 * f11 + src->m20 * f12;
    F32 t11 = src->m01 * f10 + src->m11 * f11 + src->m21 * f12;
    F32 t12 = src->m02 * f10 + src->m12 * f11 + src->m22 * f12;
    F32 t13 = src->m03 * f10 + src->m13 * f11 + src->m23 * f12;

    matrix->m20 = src->m00 * f20 + src->m10 * f21 + src->m20 * f22;
    matrix->m21 = src->m01 * f20 + src->m11 * f21 + src->m21 * f22;
    matrix->m22 = src->m02 * f20 + src->m12 * f21 + src->m22 * f22;
    matrix->m23 = src->m03 * f20 + src->m13 * f21 + src->m23 * f22;
    matrix->m00 = t00;
    matrix->m01 = t01;
    matrix->m02 = t02;
    matrix->m03 = t03;
    matrix->m10 = t10;
    matrix->m11 = t11;
    matrix->m12 = t12;
    matrix->m13 = t13;

    free(src);
}
