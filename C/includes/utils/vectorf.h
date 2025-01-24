#pragma once

#include <math.h>
#include <stdlib.h>
#include "utils/types.h"

typedef struct vectorf VECTORF;

struct vectorf {
    F32 x;
    F32 y;
    F32 z;
    F32 w;
};

U8 vectorfEquals(VECTORF* a, VECTORF* b);
void vectorfSet(VECTORF* vectorf, F32 x, F32 y, F32 z, F32 w);
void vectorfNormalize(VECTORF* vectorf);
F32 vectorfLength(VECTORF* vectorf);
void vectorfZero(VECTORF* vectorf);
void vectorfClean(VECTORF* vectorf);
VECTORF* vectorfInit();
