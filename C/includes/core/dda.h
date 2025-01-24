#pragma once

#include "utils/types.h"
#include "utils/vectori.h"
#include "utils/vectorf.h"

typedef struct ddaResult DDA_RESULT;

struct ddaResult {
    U8 blockType;
    VECTORI* position;
    VECTORI* mask;
};

void ddaClean();
void ddaInit();
void ddaZero();
DDA_RESULT* dda(VECTORF* position, VECTORF* direction, U32 maxStep);
