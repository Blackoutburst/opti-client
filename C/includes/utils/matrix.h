#pragma once

#include "utils/types.h"
#include "utils/vectorf.h"

typedef struct matrix MATRIX;

#define M00 0x00
#define M01 0x01
#define M02 0x02
#define M03 0x03

#define M10 0x04
#define M11 0x05
#define M12 0x06
#define M13 0x07

#define M20 0x08
#define M21 0x09
#define M22 0x0A
#define M23 0x0B

#define M30 0x0C
#define M31 0x0D
#define M32 0x0E
#define M33 0x0F

struct matrix {
    F32 m00;
    F32 m01;
    F32 m02;
    F32 m03;
    
    F32 m10;
    F32 m11;
    F32 m12;
    F32 m13;
    
    F32 m20;
    F32 m21;
    F32 m22;
    F32 m23;
    
    F32 m30;
    F32 m31;
    F32 m32;
    F32 m33;
};

F32* matrixGetValues(MATRIX* matrix);
void matrixSetIdentity(MATRIX* matrix);
MATRIX* identityMatrix();
F32 matrixGet(MATRIX* matrix, U32 index);
void matrixMultiply(MATRIX* result, MATRIX* left, MATRIX* right);
void matrixCopy(MATRIX* src, MATRIX* dest);
void matrixOrtho2D(MATRIX* matrix, F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far);
void matrixProjection(MATRIX* matrix, F32 width, F32 height, F32 fov, F32 near, F32 far);
void matrixScale2d(MATRIX* matrix, F32 x, F32 y);
void matrixScale2dP(MATRIX* matrix, VECTORF* vector);
void matrixScale3d(MATRIX* matrix, F32 x, F32 y, F32 z);
void matrixScale3dP(MATRIX* matrix, VECTORF* vector);
void matrixTranslate2d(MATRIX* matrix, F32 x, F32 y);
void matrixTranslate2dP(MATRIX* matrix, VECTORF* vector);
void matrixTranslate3d(MATRIX* matrix, F32 x, F32 y, F32 z);
void matrixTranslate3dP(MATRIX* matrix, VECTORF* vector);
void matrixRotate(MATRIX* matrix, F32 angle, F32 x, F32 y, F32 z);
void matrixRotateP(MATRIX* matrix, F32 angle, VECTORF* vector);

