#pragma once
#include <math.h>
#include "utils/types.h"

#define VX 0x00
#define VY 0x01
#define VZ 0x02
#define VW 0x03

#define PI 3.141592653589793

U32 xyzToIndexOobCheck(I32 x, I32 y, I32 z);
U32 xyzToIndex(I32 x, I32 y, I32 z);
void indexToXYZ(I8* vector, I16 index);
F32 rad(F32 angle);
I8 sign(I32 value);
I8 fsign(F32 value);
F32 fsignf(F32 value);
