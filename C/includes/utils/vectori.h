#pragma once

#include <math.h>
#include <stdlib.h>
#include "utils/types.h"

typedef struct vectori VECTORI;

struct vectori {
    I32 x;
    I32 y;
    I32 z;
    I32 w;
};

U8 vectoriEquals(VECTORI* a, VECTORI* b);
void vectoriSet(VECTORI* vectori, I32 x, I32 y, I32 z, I32 w);
void vectoriNormalize(VECTORI* vectori);
I32 vectoriLength(VECTORI* vectori);
void vectoriZero(VECTORI* vectori);
void vectoriClean(VECTORI* vectori);
VECTORI* vectoriInit();
