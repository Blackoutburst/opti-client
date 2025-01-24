#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils/math.h"
#include "utils/matrix.h"
#include "glfw/glfw3.h"
#include "core/camera.h"

static F64 prevMouseX = 0;
static F64 prevMouseY = 0;

void cameraGetDirection(CAMERA* camera) {
    if (camera == NULL) return;
    
    F32 rYaw = rad(camera->yaw - 90);
    F32 rPitch = rad(camera->pitch);

    F32 x = cosf(rPitch) * cosf(rYaw);
    F32 y = sinf(rPitch);
    F32 z = cosf(rPitch) * sinf(rYaw);

    F32 mag = sqrt(x * x + y * y + z * z);
    if (mag == 0.0) return;

    camera->direction->x = x / mag;
    camera->direction->y = y / mag;
    camera->direction->z = z / mag;
}

void cameraMove(CAMERA* camera) {
    if (camera == NULL) return;
    
    if (glfwGetKey(camera->window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->position->x -= sin(rad(-camera->yaw)) * CAMERA_SPEED;
        camera->position->z -= cos(rad(-camera->yaw)) * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->position->x += sin(rad(-camera->yaw)) * CAMERA_SPEED;
        camera->position->z += cos(rad(-camera->yaw)) * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->position->x += sin(rad(-camera->yaw - 90)) * CAMERA_SPEED;
        camera->position->z += cos(rad(-camera->yaw - 90)) * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->position->x += sin(rad(-camera->yaw + 90)) * CAMERA_SPEED;
        camera->position->z += cos(rad(-camera->yaw + 90)) * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera->position->y -= 1 * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera->position->y += 1 * CAMERA_SPEED;
    }
}

void cameraRotate(CAMERA* camera) {
    if (camera == NULL) return;

    F64 mouseX = 0;
    F64 mouseY = 0;
    glfwGetCursorPos(camera->window, &mouseX, &mouseY);

    F64 yawOffset = mouseX - prevMouseX;
    F64 pitchOffset = mouseY - prevMouseY;

    pitchOffset *= CAMERA_SENSITIVITY;
    yawOffset *= CAMERA_SENSITIVITY;

    prevMouseX = mouseX;
    prevMouseY = mouseY;

    camera->yaw += yawOffset;
    camera->pitch -= pitchOffset;
    if (camera->pitch > 89) camera->pitch = 89;
    if (camera->pitch < -89) camera->pitch = -89;
}

void cameraUpdate(CAMERA* camera) {
    if (camera == NULL) return;
    
    cameraMove(camera);
    cameraRotate(camera);
    cameraGetDirection(camera);
    
    matrixSetIdentity(camera->matrix);
    matrixRotate(camera->matrix, rad(camera->pitch), -1, 0, 0);
    matrixRotate(camera->matrix, rad(camera->yaw), 0, 1, 0);
    matrixTranslate3d(camera->matrix, -camera->position->x, -camera->position->y, -camera->position->z);
}

void cameraClean(CAMERA* camera) {
    if (camera == NULL) return;
    
    free(camera->matrix);
    vectorfClean(camera->position);
    vectorfClean(camera->direction);
    free(camera);
}

CAMERA* cameraInit(GLFWwindow* window) {
    VECTORF* position = vectorfInit();
    VECTORF* direction = vectorfInit();
    MATRIX* matrix = identityMatrix();
    CAMERA* camera = malloc(sizeof(CAMERA));
    camera->position = position;
    camera->yaw = 0;
    camera->pitch = 0;
    camera->window = window;
    camera->matrix = matrix;
    camera->direction = direction;

    glfwGetCursorPos(camera->window, &prevMouseX, &prevMouseY);
    
    return camera;
}
