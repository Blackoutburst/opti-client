#include "utils/vectorf.h"

U8 vectorfEquals(VECTORF* a, VECTORF* b) {
    if (a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w) return 1;
    return 0;
}

void vectorfSet(VECTORF* vectorf, F32 x, F32 y, F32 z, F32 w) {
    vectorf->x = x;
    vectorf->y = y;
    vectorf->z = z;
    vectorf->w = w;
}

void vectorfNormalize(VECTORF* vectorf) {
    F32 mag = sqrt(vectorf->x * vectorf->x + vectorf->y * vectorf->y + vectorf->z * vectorf->z + vectorf->w * vectorf->w);
    if (mag == 0.0) return;

    vectorf->x /= mag;
    vectorf->y /= mag;
    vectorf->z /= mag;
    vectorf->w /= mag;
}

F32 vectorfLength(VECTORF* vectorf) {
    return sqrt(vectorf->x * vectorf->x + vectorf->y * vectorf->y + vectorf->z * vectorf->z + vectorf->w * vectorf->w);
}

void vectorfZero(VECTORF* vectorf) {
    vectorfSet(vectorf, 0, 0, 0, 0);
}

void vectorfClean(VECTORF* vectorf) {
    free(vectorf);
}

VECTORF* vectorfInit() {
    VECTORF* vectorf = malloc(sizeof(VECTORF));
    vectorfZero(vectorf);

    return vectorf;
}
