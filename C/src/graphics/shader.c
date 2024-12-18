#define GLFW_INCLUDE_NONE

#include <stdlib.h>
#include "GLFW/glfw3.h"
#include "utils/ioUtils.h"
#include "utils/math.h"

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include "GL/glew.h"
    #include <GL/gl.h>
#else
    #include <GL/gl.h>
#endif

int createShaderProgram(int vertexShader, int fragmentShader) {
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        println(infoLog);
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int compileShader(char* source, unsigned int type) {
    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, (const char**)&source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        println(infoLog);
        return -1;
    }

    free(source);
    return shader;
}

void setUniform1i(int id, char* name, int x) {
    glProgramUniform1i(id, glGetUniformLocation(id, name), x);
}

void setUniform2i(int id, char* name, int x, int y) {
    glProgramUniform2i(id, glGetUniformLocation(id, name), x, y);
}

void setUniform2iP(int id, char* name, int* x) {
    glProgramUniform2i(id, glGetUniformLocation(id, name), x[VX], x[VY]);
}

void setUniform3i(int id, char* name, int x, int y, int z) {
    glProgramUniform3i(id, glGetUniformLocation(id, name), x, y, z);
}

void setUniform3iP(int id, char* name, int* x) {
    glProgramUniform3i(id, glGetUniformLocation(id, name), x[VX], x[VY], x[VZ]);
}

void setUniform4i(int id, char* name, int x, int y, int z, int w) {
    glProgramUniform4i(id, glGetUniformLocation(id, name), x, y, z, w);
}

void setUniform4iP(int id, char* name, int* x) {
    glProgramUniform4i(id, glGetUniformLocation(id, name), x[VX], x[VY], x[VZ], x[VW]);
}

void setUniform1f(int id, char* name, float x) {
    glProgramUniform1f(id, glGetUniformLocation(id, name), x);
}

void setUniform2f(int id, char* name, float x, float y) {
    glProgramUniform2f(id, glGetUniformLocation(id, name), x, y);
}

void setUniform2fP(int id, char* name, float* x) {
    glProgramUniform2f(id, glGetUniformLocation(id, name), x[VX], x[VY]);
}

void setUniform3f(int id, char* name, float x, float y, float z) {
    glProgramUniform3f(id, glGetUniformLocation(id, name), x, y, z);
}

void setUniform3fP(int id, char* name, float* x) {
    glProgramUniform3f(id, glGetUniformLocation(id, name), x[VX], x[VY], x[VZ]);
}

void setUniform4f(int id, char* name, float x, float y, float z, float w) {
    glProgramUniform4f(id, glGetUniformLocation(id, name), x, y, z, w);
}

void setUniform4fP(int id, char* name, float* x) {
    glProgramUniform4f(id, glGetUniformLocation(id, name), x[VX], x[VY], x[VZ], x[VW]);
}

void setUniformMat4(int id, char* name, const float* matrix) {
    glProgramUniformMatrix4fv(id, glGetUniformLocation(id, name), 1, GL_FALSE, matrix);
}


