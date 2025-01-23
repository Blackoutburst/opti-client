#define GLFW_INCLUDE_NONE

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "utils/types.h"
#include "glfw/glfw3.h"
#include "graphics/shader.h"
#include "window/window.h"
#include "utils/ioUtils.h"
#include "utils/matrix.h"
#include "utils/math.h"
#include "graphics/shader.h"
#include "graphics/textureArray.h"
#include "world/chunk.h"
#include "world/world.h"
#include "world/worldGenerator.h"
#include "network/client.h"
#include "network/networkQueue.h"
#include "network/packet.h"
#include "network/encoder.h"
#include "world/vaoQueue.h"

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include "gl/glew.h"
    #include <GL/gl.h>
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
void calculateFPS() {
    static int frameCount = 0;
    static double elapsedTime = 0.0;
    static double lastTime = 0.0;
    
    LARGE_INTEGER frequency, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&currentTime);

    double now = (double)currentTime.QuadPart / frequency.QuadPart;
    elapsedTime += now - lastTime;
    lastTime = now;

    frameCount++;

    if (elapsedTime >= 1.0) {
        printf("FPS: %d\n", frameCount);
        frameCount = 0;
        elapsedTime = 0.0;
    }
}
#endif

void update(GLFWwindow* window) {

    ////// TEXTURE ///////

    I8* textureFiles[] = {
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
        "./res/blocks/planks_oak.png",
        "./res/blocks/stonebrick.png",
        "./res/blocks/netherrack.png",
        "./res/blocks/gold_block.png",
        "./res/blocks/ice_packed.png",
        "./res/blocks/lava.png",
        "./res/blocks/barrel_top.png",
        "./res/blocks/barrel_side.png",
        "./res/blocks/barrel_bottom.png",
        "./res/blocks/bookshelf.png",
    };

    I32 diffuseMap = createTextureArray(textureFiles, 22, 16);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, diffuseMap);

    /////////////

    ////// SHADER ///////

    I8* vertexShaderSource = readFile("./res/shaders/cube.vert");
    I8* fragmentShaderSource = readFile("./res/shaders/cube.frag");

    I32 vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    I32 fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    I32 shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    MATRIX* modelMatrix = identityMatrix();
    MATRIX* viewMatrix = identityMatrix();
    MATRIX* projectionMatrix = identityMatrix();
    matrixProjection(projectionMatrix, 1280, 720, 90, 0.1, 1000);

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

    F32 x = 0;
    F32 y = 8;
    F32 z = 0;
    F64 mouseX = 0;
    F64 mouseY = 0;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    F64 prevMouseX = mouseX;
    F64 prevMouseY = mouseY;
    F64 pitch = 0;
    F64 yaw = 0;
    F64 pitchOffset = 0;
    F64 yawOffset = 0;
    F32 speed = 0.5f;
    F32 sensitivity = 0.1f;

    NET_QUEUE_ELEM* queueElement = NULL;
    VAO_QUEUE_ELEM* vaoQueueElement = NULL;

    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        //calculateFPS();

        worldRemoveChunkOutOfRenderDistance(20, x, y, z);
        
        while (networkQueuePop(&queueElement)) {
            queueElement->function(queueElement->buffer);
            networkQueueCleanElement(queueElement->id);
        }
        
        while (vaoQueuePop(&vaoQueueElement)) {
            if (vaoQueueElement->neighbor) {
                CHUNK* chunk = chunkCreate(vaoQueueElement->position, vaoQueueElement->blocks);
                chunkGenerateVAO(chunk, vaoQueueElement->mesh);
                worldAddChunk(chunk);
                vaoQueueCleanElement(vaoQueueElement->id);
            } else {
                CHUNK* chunk = chunkCreate(vaoQueueElement->position, vaoQueueElement->blocks);
                chunkGenerateVAO(chunk, vaoQueueElement->mesh);
                worldAddChunk(chunk);

                I32 chunkPoses[6][3] = {
                    {vaoQueueElement->position[VX] + CHUNK_SIZE, vaoQueueElement->position[VY], vaoQueueElement->position[VZ]},
                    {vaoQueueElement->position[VX] - CHUNK_SIZE, vaoQueueElement->position[VY], vaoQueueElement->position[VZ]},
                    {vaoQueueElement->position[VX], vaoQueueElement->position[VY] + CHUNK_SIZE, vaoQueueElement->position[VZ]},
                    {vaoQueueElement->position[VX], vaoQueueElement->position[VY] - CHUNK_SIZE, vaoQueueElement->position[VZ]},
                    {vaoQueueElement->position[VX], vaoQueueElement->position[VY], vaoQueueElement->position[VZ] + CHUNK_SIZE},
                    {vaoQueueElement->position[VX], vaoQueueElement->position[VY], vaoQueueElement->position[VZ] - CHUNK_SIZE},
                };

                for (U8 i = 0; i < 6; i++) {
                    CHUNK* tmp = worldGetChunk(chunkPoses[i][VX], chunkPoses[i][VY], chunkPoses[i][VZ]);
                    if (tmp == NULL) continue;
                    I32* position = malloc(sizeof(I32) * 3);
                    U8* blocks = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
                    memcpy(position, tmp->position, sizeof(I32) * 3);
                    memcpy(blocks, tmp->blocks, sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
                    meshQueuePush(position, blocks, 1);
                }
                vaoQueueCleanElement(vaoQueueElement->id);
            }
        }

        packetSendUpdateEntity(x, y, z, yaw, pitch);

        windowClear();

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
    
        worldRender(shaderProgram);

        windowUpdate(window);
    }

    worldClean();

    glfwTerminate();

    free(modelMatrix);
    free(viewMatrix);
    free(projectionMatrix);
}


I32 main(void) {
    worldInit();
    wgInitThreadPool();
    openConnection("162.19.137.231", 15000);
    
    GLFWwindow* window = windowCreate();

    packetSendClientMetadata(16, (const U8*)"Blackoutburst");
    
    update(window);

    wgCleanThreadPool();
    closeConnection();

    return 0;
}
