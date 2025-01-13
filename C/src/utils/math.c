#include "world/world.h"
#include "utils/types.h"

#define PI 3.141592653589793

F32 rad(F32 angle) {
    return angle * PI / 180.0f;
}

U32 xyzToIndexOobCheck(I32 x, I32 y, I32 z) {
    if (x < 0 || x > CHUNK_SIZE - 1 || y < 0 || y > CHUNK_SIZE - 1 || z < 0 || z > CHUNK_SIZE - 1)
        return -1;
    return x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
}

U32 xyzToIndex(I32 x, I32 y, I32 z) {
    return x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
}

void indexToXYZ(I8* vector, I16 index) {
    vector[0] = index % CHUNK_SIZE;
    vector[1] = (index / CHUNK_SIZE) % CHUNK_SIZE;
    vector[2] = (index / (CHUNK_SIZE * CHUNK_SIZE)) % CHUNK_SIZE;
}
