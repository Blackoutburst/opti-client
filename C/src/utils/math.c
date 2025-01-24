#include "world/world.h"
#include "utils/types.h"
#include "utils/vectori.h"
#include "utils/math.h"

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

void indexToXYZ(VECTORI* vector, I16 index) {
    vectoriSet(vector, index % CHUNK_SIZE, (index / CHUNK_SIZE) % CHUNK_SIZE, (index / (CHUNK_SIZE * CHUNK_SIZE)) % CHUNK_SIZE, 0);
}

I8 sign(I32 value) {
    if (value == 0) return 0;
    if (value < 0) return -1;
    
    return 1;
}

I8 fsign(F32 value) {
    if (value == 0.0) return 0;
    if (value < 0.0) return -1;
    
    return 1;
}

F32 fsignf(F32 value) {
    if (value == 0.0) return 0;
    if (value < 0.0) return -1;
    
    return 1;
}
