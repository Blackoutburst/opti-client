#pragma once

#include "utils/types.h"
#include "entity/entityManager.h"

void entityOtherPlayerRender(ENTITY* entity, CAMERA* camera, MATRIX* projection);
void entityOtherPlayerUpdate(ENTITY* entity, CAMERA* camera, MATRIX* projection);
void entityOtherPlayerClean(ENTITY* entity);
ENTITY* entityOtherPlayerInit(U32 id, F32 x, F32 y, F32 z, F32 yaw, F32 pitch);

