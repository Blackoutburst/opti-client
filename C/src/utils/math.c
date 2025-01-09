#include "world/world.h"

#define PI 3.141592653589793

float rad(float angle) {
    return angle * PI / 180.0f;
}

unsigned int xyzToIndexOobCheck(int x, int y, int z) {
    if (x < 0 || x > CHUNK_SIZE - 1 || y < 0 || y > CHUNK_SIZE - 1 || z < 0 || z > CHUNK_SIZE - 1)
        return -1;
    return x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
}

unsigned int xyzToIndex(int x, int y, int z) {
    return x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
}

void indexToXYZ(char* vector, short index) {
    vector[0] = index % CHUNK_SIZE;
    vector[1] = (index / CHUNK_SIZE) % CHUNK_SIZE;
    vector[2] = (index / (CHUNK_SIZE * CHUNK_SIZE)) % CHUNK_SIZE;
}
