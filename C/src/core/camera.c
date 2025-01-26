#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils/math.h"
#include "utils/matrix.h"
#include "utils/vectori.h"
#include "glfw/glfw3.h"
#include "core/camera.h"
#include "world/world.h"

static F64 prevMouseX = 0;
static F64 prevMouseY = 0;

U8 cameraIsInFrustrum(CAMERA* camera, VECTORI* position) {
    VECTORI minCorner = { position->x, position->y, position->z, 0 };
    VECTORI maxCorner = { position->x + CHUNK_SIZE, position->y + CHUNK_SIZE, position->z + CHUNK_SIZE, 0 };

    for (U8 i = 0; i < 6; i++) {
        if (camera->planes[i]->x * minCorner.x + camera->planes[i]->y * minCorner.y + camera->planes[i]->z * minCorner.z + camera->planes[i]->w > 0) continue;
        if (camera->planes[i]->x * maxCorner.x + camera->planes[i]->y * minCorner.y + camera->planes[i]->z * minCorner.z + camera->planes[i]->w > 0) continue;
        if (camera->planes[i]->x * minCorner.x + camera->planes[i]->y * maxCorner.y + camera->planes[i]->z * minCorner.z + camera->planes[i]->w > 0) continue;
        if (camera->planes[i]->x * maxCorner.x + camera->planes[i]->y * maxCorner.y + camera->planes[i]->z * minCorner.z + camera->planes[i]->w > 0) continue;
        if (camera->planes[i]->x * minCorner.x + camera->planes[i]->y * minCorner.y + camera->planes[i]->z * maxCorner.z + camera->planes[i]->w > 0) continue;
        if (camera->planes[i]->x * maxCorner.x + camera->planes[i]->y * minCorner.y + camera->planes[i]->z * maxCorner.z + camera->planes[i]->w > 0) continue;
        if (camera->planes[i]->x * minCorner.x + camera->planes[i]->y * maxCorner.y + camera->planes[i]->z * maxCorner.z + camera->planes[i]->w > 0) continue;
        if (camera->planes[i]->x * maxCorner.x + camera->planes[i]->y * maxCorner.y + camera->planes[i]->z * maxCorner.z + camera->planes[i]->w > 0) continue;
       
        return 0;
    }

    return 1;
}

void cameraUpdatePlanes(CAMERA* camera, MATRIX* projection) {
    if (camera == NULL) return;
    MATRIX* combinedMatrix = identityMatrix();
    matrixMultiply(combinedMatrix, projection, camera->matrix);

    // Right plane
    camera->planes[0]->x = combinedMatrix->m03 - combinedMatrix->m00;
    camera->planes[0]->y = combinedMatrix->m13 - combinedMatrix->m10;
    camera->planes[0]->z = combinedMatrix->m23 - combinedMatrix->m20;
    camera->planes[0]->w = combinedMatrix->m33 - combinedMatrix->m30;
    vectorfNormalize(camera->planes[0]);

    // Left plane
    camera->planes[1]->x = combinedMatrix->m03 + combinedMatrix->m00;
    camera->planes[1]->y = combinedMatrix->m13 + combinedMatrix->m10;
    camera->planes[1]->z = combinedMatrix->m23 + combinedMatrix->m20;
    camera->planes[1]->w = combinedMatrix->m33 + combinedMatrix->m30;
    vectorfNormalize(camera->planes[1]);

    // Bottom plane
    camera->planes[2]->x = combinedMatrix->m03 + combinedMatrix->m01;
    camera->planes[2]->y = combinedMatrix->m13 + combinedMatrix->m11;
    camera->planes[2]->z = combinedMatrix->m23 + combinedMatrix->m21;
    camera->planes[2]->w = combinedMatrix->m33 + combinedMatrix->m31;
    vectorfNormalize(camera->planes[2]);

    // Top plane
    camera->planes[3]->x = combinedMatrix->m03 - combinedMatrix->m01;
    camera->planes[3]->y = combinedMatrix->m13 - combinedMatrix->m11;
    camera->planes[3]->z = combinedMatrix->m23 - combinedMatrix->m21;
    camera->planes[3]->w = combinedMatrix->m33 - combinedMatrix->m31;
    vectorfNormalize(camera->planes[3]);

    // Near plane
    camera->planes[4]->x = combinedMatrix->m03 + combinedMatrix->m02;
    camera->planes[4]->y = combinedMatrix->m13 + combinedMatrix->m12;
    camera->planes[4]->z = combinedMatrix->m23 + combinedMatrix->m22;
    camera->planes[4]->w = combinedMatrix->m33 + combinedMatrix->m32;
    vectorfNormalize(camera->planes[4]);

    // Far plane
    camera->planes[5]->x = combinedMatrix->m03 - combinedMatrix->m02;
    camera->planes[5]->y = combinedMatrix->m13 - combinedMatrix->m12;
    camera->planes[5]->z = combinedMatrix->m23 - combinedMatrix->m22;
    camera->planes[5]->w = combinedMatrix->m33 - combinedMatrix->m32;
    vectorfNormalize(camera->planes[5]);
    
    free(combinedMatrix);
}

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

    camera->planes = malloc(sizeof(VECTORF) * 6);
    for (U8 i = 0; i < 6; i++) camera->planes[i] = vectorfInit();

    glfwGetCursorPos(camera->window, &prevMouseX, &prevMouseY);
    
    return camera;
}
