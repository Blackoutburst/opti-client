#pragma once
#include "utils/types.h"

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

typedef F32 MATRIX;
typedef F32 VECTOR;

void matrixSetIdentity(MATRIX* matrix);
MATRIX* identityMatrix();
void matrixCopy(MATRIX* src, MATRIX* dest);
void matrixOrtho2D(MATRIX* matrix, F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far);
void matrixProjection(MATRIX* matrix, F32 width, F32 height, F32 fov, F32 near, F32 far);
void matrixScale2d(MATRIX* matrix, F32 x, F32 y);
void matrixScale2dP(MATRIX* matrix, VECTOR* vector);
void matrixScale3d(MATRIX* matrix, F32 x, F32 y, F32 z);
void matrixScale3dP(MATRIX* matrix, VECTOR* vector);
void matrixTranslate2d(MATRIX* matrix, F32 x, F32 y);
void matrixTranslate2dP(MATRIX* matrix, VECTOR* vector);
void matrixTranslate3d(MATRIX* matrix, F32 x, F32 y, F32 z);
void matrixTranslate3dP(MATRIX* matrix, VECTOR* vector);
void matrixRotate(MATRIX* matrix, F32 angle, F32 x, F32 y, F32 z);
void matrixRotateP(MATRIX* matrix, F32 angle, VECTOR* vector);

