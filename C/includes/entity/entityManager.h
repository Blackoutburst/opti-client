#pragma once

#include "utils/types.h"
#include "utils/vectorf.h"
#include "utils/matrix.h"

#define MAX_ENTITY_COUNT 128

typedef struct entityListElement ENTITY_LIST_ELEMENT;

typedef struct entity ENTITY;

struct entity {
    MATRIX* model;
    VECTORF* position;
    F32 yaw;
    F32 pitch;
    U32 vaoId;
    U32 vboId;
    U32 eboId;
    U32 shaderProgram;
    void (*update)();
    void (*clean)();
};

struct entityListElement {
    U32 used;
    U32 id;
    ENTITY* entity;
};

void entityManagerRemoveEntity(U32 id);
void entityManagerAddEntity(U32 id, ENTITY* entity);
void entityManagerUpdateEntity();
void entityManagerClean();
void entityManagerInit();
