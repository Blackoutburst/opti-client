#pragma once

#include "utils/types.h"
#include "utils/vectorf.h"
#include "utils/matrix.h"
#include "core/camera.h"

#define MAX_ENTITY_COUNT 128

typedef struct entityListElement ENTITY_LIST_ELEMENT;

typedef struct entity ENTITY;

struct entity {
    U32 id;
    VECTORF* position;
    F32 yaw;
    F32 pitch;
    MATRIX* model;
    U32 vaoId;
    U32 vboId;
    U32 eboId;
    U32 shaderProgram;
    void (*update)(ENTITY*, CAMERA*, MATRIX*);
    void (*clean)(ENTITY*);
};

struct entityListElement {
    U32 used;
    ENTITY* entity;
};

void entityManagerUpdateEntityPosition(U32 id, F32 x, F32 y, F32 z, F32 yaw, F32 pitch);
void entityManagerRemoveEntity(U32 id);
void entityManagerAddEntity(ENTITY* entity);
void entityManagerUpdateEntity(CAMERA* camera, MATRIX* projection);
void entityManagerClean();
void entityManagerInit();
