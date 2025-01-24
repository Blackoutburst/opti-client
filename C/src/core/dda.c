#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "core/dda.h"
#include "utils/math.h"
#include "utils/vectori.h"
#include "utils/vectorf.h"
#include "world/world.h"

static DDA_RESULT* result;

static VECTORF* deltaDist;
static VECTORF* signRayDir;
static VECTORF* mapPosVec3;
static VECTORF* sideDist;

static VECTORI* mapPos;
static VECTORI* rayStep;
static VECTORI* mask;

void ddaClean() {
    vectoriClean(result->position);
    vectoriClean(result->mask);
    free(result);

    vectorfClean(deltaDist);
    vectorfClean(signRayDir);
    vectorfClean(mapPosVec3);
    vectorfClean(sideDist);

    vectoriClean(mapPos);
    vectoriClean(rayStep);
    vectoriClean(mask);
}

void ddaInit() {
    VECTORI* resultPosition = vectoriInit();
    VECTORI* resultMask = vectoriInit();
    result = malloc(sizeof(DDA_RESULT));
    result->position = resultPosition;
    result->mask = resultMask;

    deltaDist = vectorfInit();
    signRayDir = vectorfInit();
    mapPosVec3 = vectorfInit();
    sideDist = vectorfInit();

    mapPos = vectoriInit();
    rayStep = vectoriInit();
    mask = vectoriInit();

    ddaZero();
}

void ddaZero() {
    result->blockType = 0;
    
    vectoriZero(result->position);
    vectoriZero(result->mask);

    vectorfZero(deltaDist);
    vectorfZero(signRayDir);
    vectorfZero(mapPosVec3);
    vectorfZero(sideDist);

    vectoriZero(mapPos);
    vectoriZero(rayStep);
    vectoriZero(mask);
}

DDA_RESULT* dda(VECTORF* position, VECTORF* direction, U32 maxStep) {
    ddaZero();

    F32 rayDirLength = sqrt(direction->x * direction->x + direction->y * direction->y + direction->z * direction->z);
    
    vectoriSet(mapPos, floor(position->x), floor(position->y), floor(position->z), 0);
    vectorfSet(deltaDist, fabs(rayDirLength / direction->x), fabs(rayDirLength / direction->y), fabs(rayDirLength / direction->z), 0);
    vectoriSet(rayStep, fsign(direction->x), fsign(direction->y), fsign(direction->z), 0);
    vectorfSet(signRayDir, fsignf(direction->x), fsignf(direction->y), fsignf(direction->z), 0);
    vectorfSet(mapPosVec3, (F32)mapPos->x, (F32)mapPos->y, (F32)mapPos->z, 0);
    
    F32 sideDistX = (F32)(signRayDir->x * (mapPosVec3->x - position->x) + (signRayDir->x * 0.5) + 0.5) * deltaDist->x;
    F32 sideDistY = (F32)(signRayDir->y * (mapPosVec3->y - position->y) + (signRayDir->y * 0.5) + 0.5) * deltaDist->y;
    F32 sideDistZ = (F32)(signRayDir->z * (mapPosVec3->z - position->z) + (signRayDir->z * 0.5) + 0.5) * deltaDist->z;
    
    vectorfSet(sideDist, sideDistX, sideDistY, sideDistZ, 0);


    for (U32 i = 0; i < maxStep; i++) {
        U8 blockType = worldGetBlock(mapPos->x, mapPos->y, mapPos->z);
        result->blockType = blockType;
        vectoriSet(result->position, mapPos->x, mapPos->y, mapPos->z, 0);
        vectoriSet(result->mask, mask->x, mask->y, mask->z, 0);
            
        if (blockType) return result;

        if (sideDist->x < sideDist->y) {
            if (sideDist->x < sideDist->z) {
                sideDist->x += deltaDist->x;
                mapPos->x += rayStep->x;

                vectoriSet(mask, -rayStep->x, 0, 0, 0);
            } else {
                sideDist->z += deltaDist->z;
                mapPos->z += rayStep->z;

                vectoriSet(mask, 0, 0, -rayStep->z, 0);
            }
        } else {
            if (sideDist->y < sideDist->z) {
                sideDist->y += deltaDist->y;
                mapPos->y += rayStep->y;

                vectoriSet(mask, 0, -rayStep->y, 0, 0);
            } else {
                sideDist->z += deltaDist->z;
                mapPos->z += rayStep->z;
                
                vectoriSet(mask, 0, 0, -rayStep->z, 0);
            }
        }
    }

    return result;
}
