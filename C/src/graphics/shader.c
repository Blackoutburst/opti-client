#include "graphics/shader.h"

I32 createShaderProgram(I32 vertexShader, I32 fragmentShader) {
    U32 shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    I32 success;
    I8 infoLog[512];
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

I32 compileShader(I8* source, U32 type) {
    U32 shader = glCreateShader(type);

    glShaderSource(shader, 1, (const I8**)&source, NULL);
    glCompileShader(shader);

    I32 success;
    I8 infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        println(infoLog);
        return -1;
    }

    free(source);
    return shader;
}

void setUniform1i(I32 id, I8* name, I32 x) {
    glProgramUniform1i(id, glGetUniformLocation(id, name), x);
}

void setUniform2i(I32 id, I8* name, I32 x, I32 y) {
    glProgramUniform2i(id, glGetUniformLocation(id, name), x, y);
}

void setUniform2iP(I32 id, I8* name, I32* x) {
    glProgramUniform2i(id, glGetUniformLocation(id, name), x[VX], x[VY]);
}

void setUniform3i(I32 id, I8* name, I32 x, I32 y, I32 z) {
    glProgramUniform3i(id, glGetUniformLocation(id, name), x, y, z);
}

void setUniform3iP(I32 id, I8* name, I32* x) {
    glProgramUniform3i(id, glGetUniformLocation(id, name), x[VX], x[VY], x[VZ]);
}

void setUniform4i(I32 id, I8* name, I32 x, I32 y, I32 z, I32 w) {
    glProgramUniform4i(id, glGetUniformLocation(id, name), x, y, z, w);
}

void setUniform4iP(I32 id, I8* name, I32* x) {
    glProgramUniform4i(id, glGetUniformLocation(id, name), x[VX], x[VY], x[VZ], x[VW]);
}

void setUniform1f(I32 id, I8* name, F32 x) {
    glProgramUniform1f(id, glGetUniformLocation(id, name), x);
}

void setUniform2f(I32 id, I8* name, F32 x, F32 y) {
    glProgramUniform2f(id, glGetUniformLocation(id, name), x, y);
}

void setUniform2fP(I32 id, I8* name, F32* x) {
    glProgramUniform2f(id, glGetUniformLocation(id, name), x[VX], x[VY]);
}

void setUniform3f(I32 id, I8* name, F32 x, F32 y, F32 z) {
    glProgramUniform3f(id, glGetUniformLocation(id, name), x, y, z);
}

void setUniform3fP(I32 id, I8* name, F32* x) {
    glProgramUniform3f(id, glGetUniformLocation(id, name), x[VX], x[VY], x[VZ]);
}

void setUniform4f(I32 id, I8* name, F32 x, F32 y, F32 z, F32 w) {
    glProgramUniform4f(id, glGetUniformLocation(id, name), x, y, z, w);
}

void setUniform4fP(I32 id, I8* name, F32* x) {
    glProgramUniform4f(id, glGetUniformLocation(id, name), x[VX], x[VY], x[VZ], x[VW]);
}

void setUniformMat4(I32 id, I8* name, const F32* matrix) {
    glProgramUniformMatrix4fv(id, glGetUniformLocation(id, name), 1, GL_FALSE, matrix);
}


