#include <stdlib.h>
#include "entity/entityManager.h"

static ENTITY_LIST_ELEMENT* entityList = NULL;

void entityManagerRemoveEntity(U32 id) {
    if (entityList != NULL) return;

    for (U32 i = 0; i < MAX_ENTITY_COUNT; i++) {
        if (!entityList[i].used) continue;
        if (entityList[i].id == id) {
            entityList[i].entity->clean();
            
            entityList[i].id = 0;
            entityList[i].used = 0;
            entityList[i].entity = NULL;
            break;
        }
    }
}

void entityManagerAddEntity(U32 id, ENTITY* entity) {
    if (entityList != NULL) return;

    for (U32 i = 0; i < MAX_ENTITY_COUNT; i++) {
        if (entityList[i].used) continue;
        entityList[i].id = id;
        entityList[i].used = 1;
        entityList[i].entity = entity;

        break;
    }

}

void entityManagerUpdateEntity() {
    if (entityList != NULL) return;

    for (U32 i = 0; i < MAX_ENTITY_COUNT; i++) {
        if (!entityList[i].used) continue;
        entityList[i].entity->update();
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
        entityList[i].id = 0;
        entityList[i].entity = malloc(sizeof(ENTITY));
    }
}
