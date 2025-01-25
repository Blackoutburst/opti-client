#pragma once

#include "glfw/glfw3.h"
#include "utils/types.h"
#include "utils/vectorf.h"

typedef struct camera CAMERA;

#define CAMERA_SPEED 0.5
#define CAMERA_SENSITIVITY 0.1

struct camera {
    VECTORF* position;
    F32 yaw;
    F32 pitch;
    GLFWwindow* window;
    MATRIX* matrix;
    VECTORF* direction;
};

void cameraGetDirection(CAMERA* camera);
void cameraMove(CAMERA* camera);
void cameraRotate(CAMERA* camera);
void cameraUpdate(CAMERA* camera);
void cameraClean(CAMERA* camera);
CAMERA* cameraInit(GLFWwindow* window);
