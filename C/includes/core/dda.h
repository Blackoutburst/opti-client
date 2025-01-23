#pragma once

#include "utils/types.h"

typedef struct ddaResult DDA_RESULT;

struct ddaResult {
    U8 blockType;
    I32* position;
    I32* mask;
};

void ddaClean();
void ddaInit();
void ddaZero();
DDA_RESULT* dda(F32 x, F32 y, F32 z, F32* direction, U32 maxStep);
