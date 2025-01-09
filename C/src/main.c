#define GLFW_INCLUDE_NONE

#include <math.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"
#include "graphics/shader.h"
#include "window/window.h"
#include "utils/ioUtils.h"
#include "utils/matrix.h"
#include "utils/math.h"
#include "graphics/shader.h"
#include "world/chunk.h"
#include "world/world.h"

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include "GL/glew.h"
    #include <GL/gl.h>
#else
    #include <GL/gl.h>
#endif

/*
int triangle() {
    const float vertices[] = {
         0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

         0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

         0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2.0f,
         1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 2.0f,
         1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f,
         0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f,

         0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 3.0f,
         0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 3.0f,
         0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 3.0f,
         0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 3.0f,

         1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 4.0f,
         1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 4.0f,
         1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 4.0f,
         1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 4.0f,

         0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 5.0f,
         1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 5.0f,
         1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 5.0f,
         0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f
    };

    const int indices[] = {
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

    const char* vertexShaderSource = readFile("./shaders/cube.vert");
    const char* fragmentShaderSource = readFile("./shaders/cube.frag");

    int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    int shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    MATRIX modelMatrix = identityMatrix();
    MATRIX viewMatrix = identityMatrix();
    MATRIX projectionMatrix = identityMatrix();
    float angle = rad(50);
    matrixProjection(projectionMatrix, 1280, 720, 90, 0.001, 1000);
    matrixTranslate3d(modelMatrix, 0, 0, -3);
    matrixRotate(modelMatrix, angle, 1, 0, 0);
    matrixRotate(modelMatrix, angle, 0, 1, 0);
    matrixRotate(modelMatrix, angle, 0, 0, 1);

    glUseProgram(shaderProgram);
    setUniformMat4(shaderProgram, "model", modelMatrix);
    setUniformMat4(shaderProgram, "view", viewMatrix);
    setUniformMat4(shaderProgram, "projection", projectionMatrix);

    setUniform3f(shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);
    setUniform3f(shaderProgram, "viewPos", 0.0f, 0.0f, 0.0f);
    setUniform3f(shaderProgram, "lightPos", 2.0f, 2.0f, 0.0f);
    setUniform4f(shaderProgram, "color", 1.0f, 1.0f, 1.0f, 1.0f);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 24, (void*)12);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 24, (void*)20);
    glEnableVertexAttribArray(2);

    return VAO;
}
*/

void update(GLFWwindow* window) {
    //triangle();

    int* position = malloc(sizeof(int) * 3);
    char* blocks = malloc(sizeof(char) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) blocks[i] = 1;
    
    CHUNK* chunk = createChunk(position, blocks);
    int* mesh = generateChunkMesh(chunk);
    generateChunkVAO(chunk, mesh);
    cleanChunkMesh(mesh);

    /////////////

    const char* vertexShaderSource = readFile("./shaders/cube.vert");
    const char* fragmentShaderSource = readFile("./shaders/cube.frag");

    int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    int shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    MATRIX* modelMatrix = identityMatrix();
    MATRIX* viewMatrix = identityMatrix();
    MATRIX* projectionMatrix = identityMatrix();
    matrixProjection(projectionMatrix, 1280, 720, 90, 0.001, 1000);

    glUseProgram(shaderProgram);
    setUniformMat4(shaderProgram, "model", modelMatrix);
    setUniformMat4(shaderProgram, "view", viewMatrix);
    setUniformMat4(shaderProgram, "projection", projectionMatrix);

    setUniform3f(shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);
    setUniform3f(shaderProgram, "viewPos", 0.0f, 0.0f, 0.0f);
    setUniform3f(shaderProgram, "lightPos", 2.0f, 2.0f, 0.0f);
    setUniform4f(shaderProgram, "color", 1.0f, 1.0f, 1.0f, 1.0f);
    
    /////////////

    float x = -8;
    float y = -8;
    float z = -30;
    double mouseX = 0;
    double mouseY = 0;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    double prevMouseX = mouseX;
    double prevMouseY = mouseY;
    double pitch = 0;
    double yaw = 0;
    double pitchOffset = 0;
    double yawOffset = 0;
    float speed = 0.1f;
    float sensitivity = 0.1f;
    
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        clearWindow();

        matrixSetIdentity(viewMatrix);
        matrixRotate(viewMatrix, rad(yaw), 1, 0, 0);
        matrixRotate(viewMatrix, rad(pitch), 0, 1, 0);
        matrixTranslate3d(viewMatrix, -x, -y, -z);
        setUniformMat4(shaderProgram, "view", viewMatrix);

        glfwGetCursorPos(window, &mouseX, &mouseY);
        pitchOffset = mouseX - prevMouseX;
        yawOffset = mouseY - prevMouseY;

        pitchOffset *= sensitivity;
        yawOffset *= sensitivity;
        
        prevMouseX = mouseX;
        prevMouseY = mouseY;
        
        pitch += pitchOffset;
        yaw += yawOffset;
        if (yaw > 89) yaw = 89;
        if (yaw < -89) yaw = -89;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            x -= sin(rad(-pitch)) * speed;
            z -= cos(rad(-pitch)) * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            x += sin(rad(-pitch)) * speed;
            z += cos(rad(-pitch)) * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            x += sin(rad(-pitch - 90)) * speed;
            z += cos(rad(-pitch - 90)) * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            x += sin(rad(-pitch + 90)) * speed;
            z += cos(rad(-pitch + 90)) * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            y -= 1 * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            y += 1 * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        renderChunk(chunk);

        updateWindow(window);
    }

    destroyChunk(chunk);

    glfwTerminate();
}


int main(void) {
    GLFWwindow* window = createWindow();

    update(window);

    return 0;
}
