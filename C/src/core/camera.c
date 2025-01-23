#include <math.h>
#include <stdlib.h>
#include "utils/math.h"
#include "utils/matrix.h"
#include "glfw/glfw3.h"
#include "core/camera.h"

static F64 prevMouseX = 0;
static F64 prevMouseY = 0;

void cameraGetDirection(CAMERA* camera) {
    F32 rYaw = rad(camera->yaw);
    F32 rPitch = rad(camera->pitch);

    F32 x = cos(rPitch) * cos(rYaw);
    F32 y = sin(rPitch);
    F32 z = cos(rPitch) * sin(rYaw);

    F32 mag = sqrt(x * x + y * y + z * z);
    if (!mag) return;

    camera->direction[VX] = x / mag;
    camera->direction[VY] = y / mag;
    camera->direction[VZ] = z / mag;
}

void cameraMove(CAMERA* camera) {
    if (camera == NULL) return;
    
    if (glfwGetKey(camera->window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->x -= sin(rad(-camera->pitch)) * CAMERA_SPEED;
        camera->z -= cos(rad(-camera->pitch)) * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->x += sin(rad(-camera->pitch)) * CAMERA_SPEED;
        camera->z += cos(rad(-camera->pitch)) * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->x += sin(rad(-camera->pitch - 90)) * CAMERA_SPEED;
        camera->z += cos(rad(-camera->pitch - 90)) * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->x += sin(rad(-camera->pitch + 90)) * CAMERA_SPEED;
        camera->z += cos(rad(-camera->pitch + 90)) * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera->y -= 1 * CAMERA_SPEED;
    }
    if (glfwGetKey(camera->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera->y += 1 * CAMERA_SPEED;
    }
}

void cameraRotate(CAMERA* camera) {
    if (camera == NULL) return;

    F64 mouseX = 0;
    F64 mouseY = 0;
    glfwGetCursorPos(camera->window, &mouseX, &mouseY);

    F64 pitchOffset = mouseX - prevMouseX;
    F64 yawOffset = mouseY - prevMouseY;

    pitchOffset *= CAMERA_SENSITIVITY;
    yawOffset *= CAMERA_SENSITIVITY;

    prevMouseX = mouseX;
    prevMouseY = mouseY;

    camera->pitch += pitchOffset;
    camera->yaw += yawOffset;
    if (camera->yaw > 89) camera->yaw = 89;
    if (camera->yaw < -89) camera->yaw = -89;
}

void cameraUpdate(CAMERA* camera) {
    if (camera == NULL) return;
    
    cameraMove(camera);
    cameraRotate(camera);
    
    matrixSetIdentity(camera->matrix);
    matrixRotate(camera->matrix, rad(camera->yaw), 1, 0, 0);
    matrixRotate(camera->matrix, rad(camera->pitch), 0, 1, 0);
    matrixTranslate3d(camera->matrix, -camera->x, -camera->y, -camera->z);
}

void cameraClean(CAMERA* camera) {
    if (camera == NULL) return;
    
    free(camera->matrix);
    free(camera->direction);
    free(camera);
}

CAMERA* cameraInit(GLFWwindow* window) {
    F32* direction = malloc(sizeof(F32) * 3);
    direction[VX] = 0;
    direction[VY] = 0;
    direction[VZ] = 0;
    
    MATRIX* matrix = identityMatrix();
    CAMERA* camera = malloc(sizeof(CAMERA));
    camera->x = 0;
    camera->y = 0;
    camera->z = 0;
    camera->yaw = 0;
    camera->pitch = 0;
    camera->window = window;
    camera->matrix = matrix;
    camera->direction = direction;

    glfwGetCursorPos(camera->window, &prevMouseX, &prevMouseY);
    
    return camera;
}
