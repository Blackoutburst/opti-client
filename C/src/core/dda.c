#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "core/dda.h"
#include "utils/math.h"
#include "world/world.h"

static DDA_RESULT* result;

static I32* mapPos;
static F32* deltaDist;
static I32* rayStep;
static F32* signRayDir;
static F32* mapPosVec3;
static I32* mask;
static F32* sideDist;

void ddaClean() {
    free(result->position);
    free(result->mask);
    free(result);

    free(mapPos);
    free(deltaDist);
    free(rayStep);
    free(signRayDir);
    free(mapPosVec3);
    free(mask);
    free(sideDist);
}

void ddaInit() {
    result = malloc(sizeof(DDA_RESULT));
    result->position = malloc(sizeof(I32) * 3);
    result->mask = malloc(sizeof(I32) * 3);

    mapPos = malloc(sizeof(I32) * 3);
    deltaDist = malloc(sizeof(F32) * 3);
    rayStep = malloc(sizeof(I32) * 3);
    signRayDir = malloc(sizeof(F32) * 3);
    mapPosVec3 = malloc(sizeof(F32) * 3);
    mask = malloc(sizeof(I32) * 3);
    sideDist = malloc(sizeof(F32) * 3);

    ddaZero();
}

void ddaZero() {
    result->blockType = 0;
    result->position[VX] = 0;
    result->position[VY] = 0;
    result->position[VZ] = 0;
    result->mask[VX] = 0;
    result->mask[VY] = 0;
    result->mask[VZ] = 0;

    mapPos[VX] = 0;
    mapPos[VY] = 0;
    mapPos[VZ] = 0;
    
    deltaDist[VX] = 0;
    deltaDist[VY] = 0;
    deltaDist[VZ] = 0;
    
    rayStep[VX] = 0;
    rayStep[VY] = 0;
    rayStep[VZ] = 0;
    
    signRayDir[VX] = 0;
    signRayDir[VY] = 0;
    signRayDir[VZ] = 0;
    
    mapPosVec3[VX] = 0;
    mapPosVec3[VY] = 0;
    mapPosVec3[VZ] = 0;
    
    mask[VX] = 0;
    mask[VY] = 0;
    mask[VZ] = 0;
    
    sideDist[VX] = 0;
    sideDist[VY] = 0;
    sideDist[VZ] = 0;
}

DDA_RESULT* dda(F32 x, F32 y, F32 z, F32* direction, U32 maxStep) {
    ddaZero();

    mapPos[VX] = floor(x);
    mapPos[VY] = floor(y);
    mapPos[VZ] = floor(z);
    
    F32 rayDirLength = sqrt(direction[VX] * direction[VX] + direction[VY] * direction[VY] + direction[VZ] * direction[VZ]);
    
    deltaDist[VX] = fabs(rayDirLength / direction[VX]);
    deltaDist[VY] = fabs(rayDirLength / direction[VY]);
    deltaDist[VZ] = fabs(rayDirLength / direction[VZ]);
    
    rayStep[VX] = fsign(direction[VX]);
    rayStep[VY] = fsign(direction[VY]);
    rayStep[VZ] = fsign(direction[VZ]);


    signRayDir[VX] = fsignf(direction[VX]);
    signRayDir[VY] = fsignf(direction[VY]);
    signRayDir[VZ] = fsignf(direction[VZ]);

    mapPosVec3[VX] = (F32)mapPos[VX];
    mapPosVec3[VY] = (F32)mapPos[VY];
    mapPosVec3[VZ] = (F32)mapPos[VZ];
    
    sideDist[VX] = (F32)(signRayDir[VX] * (mapPosVec3[VX] - x) + (signRayDir[VX] * 0.5) + 0.5) * deltaDist[VX];
    sideDist[VY] = (F32)(signRayDir[VY] * (mapPosVec3[VY] - y) + (signRayDir[VY] * 0.5) + 0.5) * deltaDist[VY];
    sideDist[VZ] = (F32)(signRayDir[VZ] * (mapPosVec3[VZ] - z) + (signRayDir[VZ] * 0.5) + 0.5) * deltaDist[VZ];


    for (U32 i = 0; i < maxStep; i++) {
        U8 blockType = worldGetBlock(mapPos[VX], mapPos[VY], mapPos[VZ]);
        result->blockType = blockType;
        result->position[VX] = mapPos[VX];
        result->position[VY] = mapPos[VY];
        result->position[VZ] = mapPos[VZ];
        result->mask[VX] = mask[VX];
        result->mask[VY] = mask[VY];
        result->mask[VZ] = mask[VZ];
            
        if (blockType) return result;

        if (sideDist[VX] < sideDist[VY]) {
            if (sideDist[VX] < sideDist[VZ]) {
                sideDist[VX] += deltaDist[VX];
                mapPos[VX] += rayStep[VX];
                
                mask[VX] = -rayStep[VX];
                mask[VY] = 0;
                mask[VZ] = 0;
            } else {
                sideDist[VZ] += deltaDist[VZ];
                mapPos[VZ] += rayStep[VZ];
                
                mask[VX] = 0;
                mask[VY] = 0;
                mask[VZ] = -rayStep[VZ];
            }
        } else {
            if (sideDist[VY] < sideDist[VZ]) {
                sideDist[VY] += deltaDist[VY];
                mapPos[VY] += rayStep[VY];
                
                mask[VX] = 0;
                mask[VY] = -rayStep[VY];
                mask[VZ] = 0;
            } else {
                sideDist[VZ] += deltaDist[VZ];
                mapPos[VZ] += rayStep[VZ];
                
                mask[VX] = 0;
                mask[VY] = 0;
                mask[VZ] = -rayStep[VZ];
            }
        }
    }

    return result;
}
