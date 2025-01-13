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
#include "graphics/textureArray.h"
#include "world/chunk.h"
#include "world/world.h"
#include "network/client.h"

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include "GL/glew.h"
    #include <GL/gl.h>
#else
    #include <GL/gl.h>
#endif

void update(GLFWwindow* window) {
    int* position = malloc(sizeof(int) * 3);
    char* blocks = malloc(sizeof(char) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) blocks[i] = 3;

    CHUNK* chunk = createChunk(position, blocks);
    int* mesh = generateChunkMesh(chunk);
    generateChunkVAO(chunk, mesh);
    cleanChunkMesh(mesh);

    ////// TEXTURE ///////

    char* textureFiles[] = {
        "./res/blocks/error.png",
        "./res/blocks/grass_top.png",
        "./res/blocks/grass_side.png",
        "./res/blocks/dirt.png",
        "./res/blocks/stone.png",
        "./res/blocks/log_oak_top.png",
        "./res/blocks/log_oak.png",
        "./res/blocks/leaves_oak.png",
        "./res/blocks/glass.png",
        "./res/blocks/water.png",
        "./res/blocks/sand.png",
        "./res/blocks/snow.png",
    };

    int diffuseMap = createTextureArray(textureFiles, 12, 16);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, diffuseMap);

    /////////////

    ////// SHADER ///////

    const char* vertexShaderSource = readFile("./res/shaders/cube.vert");
    const char* fragmentShaderSource = readFile("./res/shaders/cube.frag");

    int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    int shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    MATRIX* modelMatrix = identityMatrix();
    MATRIX* viewMatrix = identityMatrix();
    MATRIX* projectionMatrix = identityMatrix();
    matrixProjection(projectionMatrix, 1280, 720, 90, 0.001, 1000);

    glUseProgram(shaderProgram);

    setUniform1i(shaderProgram, "diffuseMap", 0);

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

    openConnection("162.19.137.231", 15000);
    update(window);

    closeConnection();

    return 0;
}
