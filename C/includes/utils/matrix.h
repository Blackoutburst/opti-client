#pragma once
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

typedef float MATRIX;
typedef float VECTOR;

void matrixSetIdentity(MATRIX* matrix);
MATRIX* identityMatrix();
void matrixCopy(MATRIX* src, MATRIX* dest);
void matrixOrtho2D(MATRIX* matrix, float left, float right, float bottom, float top, float near, float far);
void matrixProjection(MATRIX* matrix, float width, float height, float fov, float near, float far);
void matrixScale2d(MATRIX* matrix, float x, float y);
void matrixScale2dP(MATRIX* matrix, VECTOR* vector);
void matrixScale3d(MATRIX* matrix, float x, float y, float z);
void matrixScale3dP(MATRIX* matrix, VECTOR* vector);
void matrixTranslate2d(MATRIX* matrix, float x, float y);
void matrixTranslate2dP(MATRIX* matrix, VECTOR* vector);
void matrixTranslate3d(MATRIX* matrix, float x, float y, float z);
void matrixTranslate3dP(MATRIX* matrix, VECTOR* vector);
void matrixRotate(MATRIX* matrix, float angle, float x, float y, float z);
void matrixRotateP(MATRIX* matrix, float angle, VECTOR* vector);

