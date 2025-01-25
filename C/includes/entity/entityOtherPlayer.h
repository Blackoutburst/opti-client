#pragma once

#include "utils/types.h"
#include "entity/entityManager.h"

void entityOtherPlayerRender(ENTITY* entity);
void entityOtherPlayerUpdate(ENTITY* entity);
void entityOtherPlayerClean(ENTITY* entity);
ENTITY* entityOtherPlayerInit(F32 x, F32 y, F32 z, F32 yaw, F32 pitch);

