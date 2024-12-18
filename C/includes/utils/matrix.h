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

void matrixSetIdentity(float* matrix);
float* identityMatrix();
void matrixCopy(float* src, float* dest);
void matrixOrtho2D(float* matrix, float left, float right, float bottom, float top, float near, float far);
void matrixProjection(float* matrix, float width, float height, float fov, float near, float far);
void matrixScale2d(float* matrix, float x, float y);
void matrixScale2dP(float* matrix, float* vector);
void matrixScale3d(float* matrix, float x, float y, float z);
void matrixScale3dP(float* matrix, float* vector);
void matrixTranslate2d(float* matrix, float x, float y);
void matrixTranslate2dP(float* matrix, float* vector);
void matrixTranslate3d(float* matrix, float x, float y, float z);
void matrixTranslate3dP(float* matrix, float* vector);
void matrixRotate(float* matrix, float angle, float x, float y, float z);
void matrixRotateP(float* matrix, float angle, float* vector);

