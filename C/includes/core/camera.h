#pragma once

#include "glfw/glfw3.h"
#include "utils/types.h"

typedef struct camera CAMERA;

#define CAMERA_SPEED 0.1
#define CAMERA_SENSITIVITY 0.1

struct camera {
    F32 x;
    F32 y;
    F32 z;
    F64 yaw;
    F64 pitch;
    GLFWwindow* window;
    MATRIX* matrix;
};

void cameraMove(CAMERA* camera);
void cameraRotate(CAMERA* camera);
void cameraUpdate(CAMERA* camera);
void cameraClean(CAMERA* camera);
CAMERA* cameraInit(GLFWwindow* window);
