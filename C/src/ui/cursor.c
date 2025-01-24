#include "ui/cursor.h"
#include "utils/ioUtils.h"
#include "utils/matrix.h"
#include "graphics/opengl.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "world/blocks.h"

static F32 vertices[] = {
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 1, 0, 1,
    1, 0, 1, 0,
};

static I32 indices[] = {
    0, 1, 2,
    0, 3, 1,
};

static U32 texture = 0;
static U32 shaderProgram = 0;
static U32 vaoID = 0;
static U32 vboID = 0;
static U32 eboID = 0;
static MATRIX* model = NULL;
static MATRIX* projection = NULL;

void cursorRender(U8 blockType, U32 blockTextures) {
    U8 layer = blockType == GRASS ? blocksTextureFace(blockType, 0) : blocksTextureFace(blockType, 1);

    glUseProgram(shaderProgram);
    setUniform1i(shaderProgram, "text", 0);
    setUniform1i(shaderProgram, "diffuseMap", 1);
    setUniform1f(shaderProgram, "layer", layer);
    setUniformMat4(shaderProgram, "model", model);
    setUniformMat4(shaderProgram, "projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, blockTextures);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vaoID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void cursorClean() {
    textureDelete(texture);
    deleteShaderProgram(shaderProgram);
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
    free(model);
    free(projection);
}

void cursorInit() {
    model = identityMatrix();
    matrixScale2d(model, 1280, 720);

    projection = identityMatrix();
    matrixOrtho2D(projection, 0, 1280, 0, 720, -1, 1);
    
    texture = textureCreate("./res/awp.png", 1);
    I8* vertexShaderSource = readFile("./res/shaders/2D.vert");
    I8* fragmentShaderSource = readFile("./res/shaders/AWP.frag");
    I32 vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    I32 fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
    glGenBuffers(1, &eboID);

    glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(F32) * 16, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(I32) * 6, indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (const void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (const void *)8);
}
