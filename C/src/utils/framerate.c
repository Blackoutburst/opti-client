#include "utils/framerate.h"

#if defined(_WIN32) || defined(_WIN64)
void FPS() {
    static I32 frameCount = 0;
    static F64 elapsedTime = 0.0;
    static F64 lastTime = 0.0;
    
    LARGE_INTEGER frequency, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&currentTime);

    F64 now = (F64)currentTime.QuadPart / frequency.QuadPart;
    elapsedTime += now - lastTime;
    lastTime = now;

    frameCount++;

    if (elapsedTime >= 1.0) {
        printf("FPS: %d\n", frameCount);
        frameCount = 0;
        elapsedTime = 0.0;
    }
}
#else
void FPS() {
    static I32 frameCount = 0;
    static F64 elapsedTime = 0.0;
    static struct timespec lastTime = {0, 0};

    struct timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);

    F64 now = currentTime.tv_sec + (currentTime.tv_nsec / 1e9);
    F64 last = lastTime.tv_sec + (lastTime.tv_nsec / 1e9);
    elapsedTime += now - last;
    lastTime = currentTime;

    frameCount++;

    if (elapsedTime >= 1.0) {
        printf("FPS: %d\n", frameCount);
        frameCount = 0;
        elapsedTime = 0.0;
    }
}
#endif
