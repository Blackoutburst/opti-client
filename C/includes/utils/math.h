#include <math.h>

#define VX 0x00
#define VY 0x01
#define VZ 0x02
#define VW 0x03

#define PI 3.141592653589793

typedef float* VECTOR;

unsigned int xyzToIndex(int x, int y, int z);
void indexToXYZ(char* vector, short index);
float rad(float angle);
