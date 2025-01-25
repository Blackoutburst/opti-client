#include "graphics/opengl.h"
#include "graphics/shader.h"
#include "utils/ioUtils.h"
#include "utils/vectorf.h"
#include "utils/matrix.h"
#include "utils/math.h"
#include "entity/entityOtherPlayer.h"
#include "entity/entityManager.h"

const F32 vertices[] = {
     0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 0.0, 1.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 0.0, 1.0, 0.0,

     0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0, 1.0,
     1.0, 1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 1.0, 0.0, 1.0, 0.0, 1.0,

     0.0, 0.0, 1.0, 0.0, 1.0, 2.0,
     1.0, 0.0, 1.0, 1.0, 1.0, 2.0,
     1.0, 1.0, 1.0, 1.0, 0.0, 2.0,
     0.0, 1.0, 1.0, 0.0, 0.0, 2.0,

     0.0, 0.0, 0.0, 1.0, 1.0, 3.0,
     0.0, 1.0, 0.0, 1.0, 0.0, 3.0,
     0.0, 1.0, 1.0, 0.0, 0.0, 3.0,
     0.0, 0.0, 1.0, 0.0, 1.0, 3.0,

     1.0, 0.0, 0.0, 0.0, 1.0, 4.0,
     1.0, 1.0, 0.0, 0.0, 0.0, 4.0,
     1.0, 1.0, 1.0, 1.0, 0.0, 4.0,
     1.0, 0.0, 1.0, 1.0, 1.0, 4.0,

     0.0, 0.0, 0.0, 0.0, 1.0, 5.0,
     1.0, 0.0, 0.0, 1.0, 1.0, 5.0,
     1.0, 0.0, 1.0, 1.0, 0.0, 5.0,
     0.0, 0.0, 1.0, 0.0, 0.0, 5.0
};

const I32 indices[] = {
    0, 2, 1,
    0, 3, 2,

    4, 6, 5,
    4, 7, 6,

    9,  10, 8,
    10, 11, 8,

    12, 14, 13,
    12, 15, 14,

    17, 18, 16,
    18, 19, 16,

    21, 22, 20,
    22, 23, 20
};

void entityOtherPlayerRender(ENTITY* entity, CAMERA* camera, MATRIX* projection) {
    if (entity == NULL) return;

    glUseProgram(entity->shaderProgram);

    setUniformMat4(entity->shaderProgram, "model", entity->model);
    setUniformMat4(entity->shaderProgram, "view", camera->matrix);
    setUniformMat4(entity->shaderProgram, "projection", projection);
    
    glBindVertexArray(entity->vaoId);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); 
}

void entityOtherPlayerUpdate(ENTITY* entity, CAMERA* camera, MATRIX* projection) {
    if (entity == NULL) return;

    matrixSetIdentity(entity->model);
    matrixTranslate3d(entity->model, entity->position->x, entity->position->y, entity->position->z);
    matrixRotate(entity->model, entity->yaw, 0, -1, 0);
    matrixRotate(entity->model, entity->pitch, 1, 0, 0);
    
    
    entityOtherPlayerRender(entity, camera, projection);
}

void entityOtherPlayerClean(ENTITY* entity) {
    if (entity == NULL) return;
    
    deleteShaderProgram(entity->shaderProgram);
    glDeleteVertexArrays(1, &entity->vaoId);
    glDeleteBuffers(1, &entity->vboId);
    glDeleteBuffers(1, &entity->eboId);

    vectorfClean(entity->position);
    free(entity->model);
    free(entity);
}

ENTITY* entityOtherPlayerInit(U32 id, F32 x, F32 y, F32 z, F32 yaw, F32 pitch) {
    ENTITY* entity = malloc(sizeof(ENTITY));
    entity->id = id;

    entity->update = &entityOtherPlayerUpdate;
    entity->clean = &entityOtherPlayerClean;
    
    VECTORF* position = vectorfInit();
    vectorfSet(position, x, y, z, 0);
    entity->position = position;
    
    MATRIX* model = identityMatrix();
    entity->model = model;
    
    entity->yaw = yaw;
    entity->pitch = pitch;

    // ASS
    I8* vertexShaderSource = readFile("./res/shaders/cube.vert");
    I8* fragmentShaderSource = readFile("./res/shaders/cube.frag");
    I32 vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    I32 fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    // End of ass section
    
    entity->shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    glGenVertexArrays(1, &entity->vaoId);
    glBindVertexArray(entity->vaoId);

    glGenBuffers(1, &entity->vboId);
    glBindBuffer(GL_ARRAY_BUFFER, entity->vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &entity->eboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entity->eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 24, (void*)12);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 24, (void*)20);
    glEnableVertexAttribArray(2);

    return entity;
}

