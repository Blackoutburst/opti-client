#define GLFW_INCLUDE_NONE

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "utils/types.h"
#include "glfw/glfw3.h"
#include "graphics/shader.h"
#include "graphics/shader.h"
#include "graphics/textureArray.h"
#include "graphics/opengl.h"
#include "window/window.h"
#include "utils/ioUtils.h"
#include "utils/matrix.h"
#include "utils/math.h"
#include "world/chunk.h"
#include "world/world.h"
#include "world/worldGenerator.h"
#include "network/client.h"
#include "network/networkQueue.h"
#include "network/packet.h"
#include "network/encoder.h"
#include "world/vaoQueue.h"
#include "core/camera.h"
#include "core/dda.h"
#include "utils/framerate.h"

#if defined(_WIN32) || defined(_WIN64)
void sendPosition(F32 x, F32 y, F32 z, F32 yaw, F32 pitch) {
    static F64 elapsedTime = 0.0;
    static F64 lastTime = 0.0;
    
    LARGE_INTEGER frequency;
    LARGE_INTEGER currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&currentTime);

    F64 now = (F64)currentTime.QuadPart / frequency.QuadPart;
    elapsedTime += now - lastTime;
    lastTime = now;

    if (elapsedTime >= 0.05) {
        packetSendUpdateEntity(x, y, z, yaw, pitch);
        elapsedTime = 0.0;
    }
}
#else
void sendPosition(F32 x, F32 y, F32 z, F32 yaw, F32 pitch) {
    static F64 elapsedTime = 0.0;
    static struct timespec lastTime = {0, 0};

    struct timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);

    F64 now = currentTime.tv_sec + (currentTime.tv_nsec / 1e9);
    F64 last = lastTime.tv_sec + (lastTime.tv_nsec / 1e9);
    elapsedTime += now - last;
    lastTime = currentTime;

    if (elapsedTime >= 0.05) {
        packetSendUpdateEntity(x, y, z, yaw, pitch);
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

    CAMERA* camera = cameraInit(window);

    I8* vertexShaderSource = readFile("./res/shaders/cube.vert");
    I8* fragmentShaderSource = readFile("./res/shaders/cube.frag");

    I32 vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    I32 fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    I32 shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    MATRIX* modelMatrix = identityMatrix();
    MATRIX* projectionMatrix = identityMatrix();
    matrixProjection(projectionMatrix, 1280, 720, 90, 0.1, 1000);

    glUseProgram(shaderProgram);

    setUniform1i(shaderProgram, "diffuseMap", 0);

    setUniformMat4(shaderProgram, "model", modelMatrix);
    setUniformMat4(shaderProgram, "view", camera->matrix);
    setUniformMat4(shaderProgram, "projection", projectionMatrix);

    setUniform3f(shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);
    setUniform3f(shaderProgram, "viewPos", 0.0f, 0.0f, 0.0f);
    setUniform3f(shaderProgram, "lightPos", 2.0f, 2.0f, 0.0f);
    setUniform4f(shaderProgram, "color", 1.0f, 1.0f, 1.0f, 1.0f);

    /////////////

    ddaInit();

    NET_QUEUE_ELEM* queueElement = NULL;
    VAO_QUEUE_ELEM* vaoQueueElement = NULL;

    U8 ld = 0;
    U8 rd = 0;
    U8 md = 0;

    U8 selectedBlockType = 3;

    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        //calculateFPS();

        worldRemoveChunkOutOfRenderDistance(20, camera->position->x, camera->position->y, camera->position->z);
        
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
                    {vaoQueueElement->position->x + CHUNK_SIZE, vaoQueueElement->position->y, vaoQueueElement->position->z},
                    {vaoQueueElement->position->x - CHUNK_SIZE, vaoQueueElement->position->y, vaoQueueElement->position->z},
                    {vaoQueueElement->position->x, vaoQueueElement->position->y + CHUNK_SIZE, vaoQueueElement->position->z},
                    {vaoQueueElement->position->x, vaoQueueElement->position->y - CHUNK_SIZE, vaoQueueElement->position->z},
                    {vaoQueueElement->position->x, vaoQueueElement->position->y, vaoQueueElement->position->z + CHUNK_SIZE},
                    {vaoQueueElement->position->x, vaoQueueElement->position->y, vaoQueueElement->position->z - CHUNK_SIZE},
                };

                for (U8 i = 0; i < 6; i++) {
                    CHUNK* tmp = worldGetChunk(chunkPoses[i][0], chunkPoses[i][1], chunkPoses[i][2]);
                    if (tmp == NULL) continue;
                    VECTORI* position = vectoriInit();
                    vectoriSet(position, tmp->position->x, tmp->position->y, tmp->position->z, tmp->position->w);
                    U8* blocks = malloc(sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
                    memcpy(blocks, tmp->blocks, sizeof(U8) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
                    meshQueuePush(position, blocks, 1);
                }
                vaoQueueCleanElement(vaoQueueElement->id);
            }
        }

        sendPosition(camera->position->x, camera->position->y, camera->position->z, camera->yaw, camera->pitch);

        windowClear();

        cameraUpdate(camera);

        setUniformMat4(shaderProgram, "view", camera->matrix);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !ld) {
            DDA_RESULT* ddaResult = dda(camera->position, camera->direction, 20);
            packetSendUpdateBlock(0, ddaResult->position->x, ddaResult->position->y, ddaResult->position->z);
        }
        ld = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !rd) {
            DDA_RESULT* ddaResult = dda(camera->position, camera->direction, 20);
            packetSendUpdateBlock(selectedBlockType, ddaResult->position->x + ddaResult->mask->x, ddaResult->position->y + ddaResult->mask->y, ddaResult->position->z + ddaResult->mask->z);
        }
        rd = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS && !md) {
            DDA_RESULT* ddaResult = dda(camera->position, camera->direction, 20);
            selectedBlockType = ddaResult->blockType;
        }
        md = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

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

    ddaClean();

    cameraClean(camera);

    glfwTerminate();

    free(modelMatrix);
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
