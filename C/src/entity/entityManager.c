#include <stdlib.h>
#include "utils/ioUtils.h"
#include "entity/entityManager.h"
#include "utils/matrix.h"
#include "core/camera.h"

static ENTITY_LIST_ELEMENT* entityList = NULL;

void entityManagerUpdateEntityPosition(U32 id, F32 x, F32 y, F32 z, F32 yaw, F32 pitch) {
    if (entityList == NULL) return;

    for (U32 i = 0; i < MAX_ENTITY_COUNT; i++) {
        if (!entityList[i].used) continue;
        if (entityList[i].entity->id == id) {
            entityList[i].entity->position->x = x;
            entityList[i].entity->position->y = y;
            entityList[i].entity->position->z = z;
            entityList[i].entity->yaw = yaw;
            entityList[i].entity->pitch = pitch;
            break;
        }
    }
}

void entityManagerRemoveEntity(U32 id) {
    if (entityList == NULL) return;

    for (U32 i = 0; i < MAX_ENTITY_COUNT; i++) {
        if (!entityList[i].used) continue;
        if (entityList[i].entity->id == id) {
            entityList[i].entity->clean(entityList[i].entity);
            
            entityList[i].used = 0;
            entityList[i].entity = NULL;
            break;
        }
    }
}

void entityManagerAddEntity(ENTITY* entity) {
    if (entityList == NULL) return;

    for (U32 i = 0; i < MAX_ENTITY_COUNT; i++) {
        if (entityList[i].used) continue;
        entityList[i].used = 1;
        entityList[i].entity = entity;

        break;
    }

}

void entityManagerUpdateEntity(CAMERA* camera, MATRIX* projection) {
    if (entityList == NULL) return;

    for (U32 i = 0; i < MAX_ENTITY_COUNT; i++) {
        if (!entityList[i].used) continue;
        entityList[i].entity->update(entityList[i].entity, camera, projection);
    }
}

void entityManagerClean() {
    if (entityList == NULL) return;
    for (U32 i = 0; i < MAX_ENTITY_COUNT; i++) free(entityList[i].entity);
    free(entityList);
}

void entityManagerInit() {
    if (entityList != NULL) return;

    entityList = malloc(sizeof(ENTITY_LIST_ELEMENT) * MAX_ENTITY_COUNT);
    for (U32 i = 0; i < MAX_ENTITY_COUNT; i++) {
        entityList[i].used = 0;
        entityList[i].entity = malloc(sizeof(ENTITY));
    }
}
