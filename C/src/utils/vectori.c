#include "utils/vectori.h"

U8 vectoriEquals(VECTORI* a, VECTORI* b) {
    if (a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w) return 1;
    return 0;
}

void vectoriSet(VECTORI* vectori, I32 x, I32 y, I32 z, I32 w) {
    vectori->x = x;
    vectori->y = y;
    vectori->z = z;
    vectori->w = w;
}

void vectoriNormalize(VECTORI* vectori) {
    I32 mag = sqrt(vectori->x * vectori->x + vectori->y * vectori->y + vectori->z * vectori->z + vectori->w * vectori->w);
    if (mag == 0.0) return;

    vectori->x /= mag;
    vectori->x /= mag;
    vectori->x /= mag;
    vectori->w /= mag;
}

I32 vectoriLength(VECTORI* vectori) {
    return sqrt(vectori->x * vectori->x + vectori->y * vectori->y + vectori->z * vectori->z + vectori->w * vectori->w);
}

void vectoriZero(VECTORI* vectori) {
    vectoriSet(vectori, 0, 0, 0, 0);
}

void vectoriClean(VECTORI* vectori) {
    free(vectori);
}

VECTORI* vectoriInit() {
    VECTORI* vectori = malloc(sizeof(VECTORI));
    vectoriZero(vectori);

    return vectori;
}
