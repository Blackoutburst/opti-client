#include <stdlib.h>
#include "utils/math.h"
#include "utils/matrix.h"
#include "utils/types.h"

void matrixSetIdentity(MATRIX* matrix) {
    matrix[M00] = 1.0f;
    matrix[M01] = 0.0f;
    matrix[M02] = 0.0f;
    matrix[M03] = 0.0f;

    matrix[M10] = 0.0f;
    matrix[M11] = 1.0f;
    matrix[M12] = 0.0f;
    matrix[M13] = 0.0f;

    matrix[M20] = 0.0f;
    matrix[M21] = 0.0f;
    matrix[M22] = 1.0f;
    matrix[M23] = 0.0f;

    matrix[M30] = 0.0f;
    matrix[M31] = 0.0f;
    matrix[M32] = 0.0f;
    matrix[M33] = 1.0f;
}

MATRIX* identityMatrix() {
    MATRIX* matrix = (MATRIX*)malloc(16 * 4);
    matrixSetIdentity(matrix);

    return matrix;
}

void matrixCopy(MATRIX* src, MATRIX* dest) {
    dest[M00] = src[M00];
    dest[M01] = src[M01];
    dest[M02] = src[M02];
    dest[M03] = src[M03];

    dest[M10] = src[M10];
    dest[M11] = src[M11];
    dest[M12] = src[M12];
    dest[M13] = src[M13];

    dest[M20] = src[M20];
    dest[M21] = src[M21];
    dest[M22] = src[M22];
    dest[M23] = src[M23];

    dest[M30] = src[M30];
    dest[M31] = src[M31];
    dest[M32] = src[M32];
    dest[M33] = src[M33];
}

void matrixOrtho2D(MATRIX* matrix, F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far) {
    F32 xOrt = 2.0f / (right - left);
    F32 yOrt = 2.0f / (top - bottom);
    F32 zOrt = -2.0f / (far - near);

    F32 tx = -(right + left) / (right - left);
    F32 ty = -(top + bottom) / (top - bottom);
    F32 tz = -(far + near) / (far - near);

    matrix[M00] = xOrt;
    matrix[M10] = 0.0f;
    matrix[M20] = 0.0f;
    matrix[M30] = tx;

    matrix[M01] = 0.0f;
    matrix[M11] = yOrt;
    matrix[M21] = 0.0f;
    matrix[M31] = ty;

    matrix[M02] = 0.0f;
    matrix[M12] = 0.0f;
    matrix[M22] = zOrt;
    matrix[M32] = tz;

    matrix[M03] = 0.0f;
    matrix[M13] = 0.0f;
    matrix[M23] = 0.0f;
    matrix[M33] = 1.0f;
}

void matrixProjection(MATRIX* matrix, F32 width, F32 height, F32 fov, F32 near, F32 far) {
    F32 aspectRatio = width / height;
    F32 yScale = 1.0f / tan(rad(fov / 2.0f));
    F32 xScale = yScale / aspectRatio;
    F32 frustumLength = far - near;

    matrix[M00] = xScale;
    matrix[M11] = yScale;
    matrix[M22] = -((far + near) / frustumLength);
    matrix[M23] = -1.0f;
    matrix[M32] = -((2.0f * near * far) / frustumLength);
    matrix[M33] = 0.0f;
}

void matrixScale2d(MATRIX* matrix, F32 x, F32 y) {
    matrix[M00] = matrix[M00] * x;
    matrix[M01] = matrix[M01] * x;
    matrix[M02] = matrix[M02] * x;
    matrix[M03] = matrix[M03] * x;
    matrix[M10] = matrix[M10] * y;
    matrix[M11] = matrix[M11] * y;
    matrix[M12] = matrix[M12] * y;
    matrix[M13] = matrix[M13] * y;
}

void matrixScale2dP(MATRIX* matrix, VECTOR* vector) {
    matrix[M00] = matrix[M00] * vector[VX];
    matrix[M01] = matrix[M01] * vector[VX];
    matrix[M02] = matrix[M02] * vector[VX];
    matrix[M03] = matrix[M03] * vector[VX];
    matrix[M10] = matrix[M10] * vector[VY];
    matrix[M11] = matrix[M11] * vector[VY];
    matrix[M12] = matrix[M12] * vector[VY];
    matrix[M13] = matrix[M13] * vector[VY];
}

void matrixScale3d(MATRIX* matrix, F32 x, F32 y, F32 z) {
    matrix[M00] = matrix[M00] * x;
    matrix[M01] = matrix[M01] * x;
    matrix[M02] = matrix[M02] * x;
    matrix[M03] = matrix[M03] * x;
    matrix[M10] = matrix[M10] * y;
    matrix[M11] = matrix[M11] * y;
    matrix[M12] = matrix[M12] * y;
    matrix[M13] = matrix[M13] * y;
    matrix[M20] = matrix[M20] * z;
    matrix[M21] = matrix[M21] * z;
    matrix[M22] = matrix[M22] * z;
    matrix[M23] = matrix[M23] * z;
}

void matrixScale3dP(MATRIX* matrix, VECTOR* vector) {
    matrix[M00] = matrix[M00] * vector[VX];
    matrix[M01] = matrix[M01] * vector[VX];
    matrix[M02] = matrix[M02] * vector[VX];
    matrix[M03] = matrix[M03] * vector[VX];
    matrix[M10] = matrix[M10] * vector[VY];
    matrix[M11] = matrix[M11] * vector[VY];
    matrix[M12] = matrix[M12] * vector[VY];
    matrix[M13] = matrix[M13] * vector[VY];
    matrix[M20] = matrix[M20] * vector[VZ];
    matrix[M21] = matrix[M21] * vector[VZ];
    matrix[M22] = matrix[M22] * vector[VZ];
    matrix[M23] = matrix[M23] * vector[VZ];
}

void matrixTranslate2d(MATRIX* matrix, F32 x, F32 y) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    matrix[M30] += src[M00] * x + src[M10] * y;
    matrix[M31] += src[M01] * x + src[M11] * y;
    matrix[M32] += src[M02] * x + src[M12] * y;
    matrix[M33] += src[M03] * x + src[M13] * y;

    free(src);
}

void matrixTranslate2dP(MATRIX* matrix, VECTOR* vector) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    matrix[M30] += src[M00] * vector[VX] + src[M10] * vector[VY];
    matrix[M31] += src[M01] * vector[VX] + src[M11] * vector[VY];
    matrix[M32] += src[M02] * vector[VX] + src[M12] * vector[VY];
    matrix[M33] += src[M03] * vector[VX] + src[M13] * vector[VY];

    free(src);
}

void matrixTranslate3d(MATRIX* matrix, F32 x, F32 y, F32 z) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    matrix[M30] += src[M00] * x + src[M10] * y + src[M20] * z;
    matrix[M31] += src[M01] * x + src[M11] * y + src[M21] * z;
    matrix[M32] += src[M02] * x + src[M12] * y + src[M22] * z;
    matrix[M33] += src[M03] * x + src[M13] * y + src[M23] * z;

    free(src);
}

void matrixTranslate3dP(MATRIX* matrix, VECTOR* vector) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    matrix[M30] += src[M00] * vector[VX] + src[M10] * vector[VY] + src[M20] * vector[VZ];
    matrix[M31] += src[M01] * vector[VX] + src[M11] * vector[VY] + src[M21] * vector[VZ];
    matrix[M32] += src[M02] * vector[VX] + src[M12] * vector[VY] + src[M22] * vector[VZ];
    matrix[M33] += src[M03] * vector[VX] + src[M13] * vector[VY] + src[M23] * vector[VZ];

    free(src);
}

void matrixRotateP(MATRIX* matrix, F32 angle, VECTOR* vector) {
    MATRIX* src = identityMatrix();
    matrixCopy(matrix, src);

    F32 c = cos(angle);
    F32 s = sin(angle);
    F32 oneMinusC = 1.0f - c;
    F32 xy = vector[VX] * vector[VY];
    F32 yz = vector[VY] * vector[VZ];
    F32 xz = vector[VX] * vector[VZ];
    F32 xs = vector[VX] * s;
    F32 ys = vector[VY] * s;
    F32 zs = vector[VZ] * s;

    F32 f00 = vector[VX] * vector[VX] * oneMinusC + c;
    F32 f01 = xy * oneMinusC + zs;
    F32 f02 = xz * oneMinusC - ys;
    F32 f10 = xy * oneMinusC - zs;
    F32 f11 = vector[VY] * vector[VY] * oneMinusC + c;
    F32 f12 = yz * oneMinusC + xs;
    F32 f20 = xz * oneMinusC + ys;
    F32 f21 = yz * oneMinusC - xs;
    F32 f22 = vector[VZ] * vector[VZ] * oneMinusC + c;

    F32 t00 = src[M00] * f00 + src[M10] * f01 + src[M20] * f02;
    F32 t01 = src[M01] * f00 + src[M11] * f01 + src[M21] * f02;
    F32 t02 = src[M02] * f00 + src[M12] * f01 + src[M22] * f02;
    F32 t03 = src[M03] * f00 + src[M13] * f01 + src[M23] * f02;
    F32 t10 = src[M00] * f10 + src[M10] * f11 + src[M20] * f12;
    F32 t11 = src[M01] * f10 + src[M11] * f11 + src[M21] * f12;
    F32 t12 = src[M02] * f10 + src[M12] * f11 + src[M22] * f12;
    F32 t13 = src[M03] * f10 + src[M13] * f11 + src[M23] * f12;

    matrix[M20] = src[M00] * f20 + src[M10] * f21 + src[M20] * f22;
    matrix[M21] = src[M01] * f20 + src[M11] * f21 + src[M21] * f22;
    matrix[M22] = src[M02] * f20 + src[M12] * f21 + src[M22] * f22;
    matrix[M23] = src[M03] * f20 + src[M13] * f21 + src[M23] * f22;
    matrix[M00] = t00;
    matrix[M01] = t01;
    matrix[M02] = t02;
    matrix[M03] = t03;
    matrix[M10] = t10;
    matrix[M11] = t11;
    matrix[M12] = t12;
    matrix[M13] = t13;

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

    F32 t00 = src[M00] * f00 + src[M10] * f01 + src[M20] * f02;
    F32 t01 = src[M01] * f00 + src[M11] * f01 + src[M21] * f02;
    F32 t02 = src[M02] * f00 + src[M12] * f01 + src[M22] * f02;
    F32 t03 = src[M03] * f00 + src[M13] * f01 + src[M23] * f02;
    F32 t10 = src[M00] * f10 + src[M10] * f11 + src[M20] * f12;
    F32 t11 = src[M01] * f10 + src[M11] * f11 + src[M21] * f12;
    F32 t12 = src[M02] * f10 + src[M12] * f11 + src[M22] * f12;
    F32 t13 = src[M03] * f10 + src[M13] * f11 + src[M23] * f12;

    matrix[M20] = src[M00] * f20 + src[M10] * f21 + src[M20] * f22;
    matrix[M21] = src[M01] * f20 + src[M11] * f21 + src[M21] * f22;
    matrix[M22] = src[M02] * f20 + src[M12] * f21 + src[M22] * f22;
    matrix[M23] = src[M03] * f20 + src[M13] * f21 + src[M23] * f22;
    matrix[M00] = t00;
    matrix[M01] = t01;
    matrix[M02] = t02;
    matrix[M03] = t03;
    matrix[M10] = t10;
    matrix[M11] = t11;
    matrix[M12] = t12;
    matrix[M13] = t13;

    free(src);
}
