#pragma once

#include "glfw/glfw3.h"
#include "utils/types.h"
#include "utils/vectorf.h"
#include "utils/vectori.h"
#include "utils/matrix.h"

typedef struct camera CAMERA;

#define CAMERA_SPEED 0.1
#define CAMERA_SENSITIVITY 0.1

struct camera {
    VECTORF** planes;
    VECTORF* position;
    F32 yaw;
    F32 pitch;
    GLFWwindow* window;
    MATRIX* matrix;
    VECTORF* direction;
};

U8 cameraIsInFrustrum(CAMERA* camera, VECTORI* position);
void cameraUpdatePlanes(CAMERA* camera, MATRIX* projection);
void cameraGetDirection(CAMERA* camera);
void cameraMove(CAMERA* camera);
void cameraRotate(CAMERA* camera);
void cameraUpdate(CAMERA* camera);
void cameraClean(CAMERA* camera);
CAMERA* cameraInit(GLFWwindow* window);
