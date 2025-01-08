#define PI 3.141592653589793

float rad(float angle) {
    return angle * PI / 180.0f;
}

void indexToXYZ(char* vector, short index) {
    vector[0] = index % 16;
    vector[1] = (index / 16) % 16;
    vector[2] = (index / (16 * 16)) % 16;
}
