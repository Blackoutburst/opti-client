#include "graphics/shader.h"

void deleteShaderProgram(U32 id) {
    glDeleteProgram(id);
}

U32 createShaderProgram(U32 vertexShader, U32 fragmentShader) {
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

U32 compileShader(I8* source, U32 type) {
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

void setUniform1i(U32 id, I8* name, I32 x) {
    glProgramUniform1i(id, glGetUniformLocation(id, name), x);
}

void setUniform2i(U32 id, I8* name, I32 x, I32 y) {
    glProgramUniform2i(id, glGetUniformLocation(id, name), x, y);
}

void setUniform2iP(U32 id, I8* name, VECTORI* vector) {
    glProgramUniform2i(id, glGetUniformLocation(id, name), vector->x, vector->y);
}

void setUniform3i(U32 id, I8* name, I32 x, I32 y, I32 z) {
    glProgramUniform3i(id, glGetUniformLocation(id, name), x, y, z);
}

void setUniform3iP(U32 id, I8* name, VECTORI* vector) {
    glProgramUniform3i(id, glGetUniformLocation(id, name), vector->x, vector->y, vector->z);
}

void setUniform4i(U32 id, I8* name, I32 x, I32 y, I32 z, I32 w) {
    glProgramUniform4i(id, glGetUniformLocation(id, name), x, y, z, w);
}

void setUniform4iP(U32 id, I8* name, VECTORI* vector) {
    glProgramUniform4i(id, glGetUniformLocation(id, name), vector->x, vector->y, vector->z, vector->w);
}

void setUniform1f(U32 id, I8* name, F32 x) {
    glProgramUniform1f(id, glGetUniformLocation(id, name), x);
}

void setUniform2f(U32 id, I8* name, F32 x, F32 y) {
    glProgramUniform2f(id, glGetUniformLocation(id, name), x, y);
}

void setUniform2fP(U32 id, I8* name, VECTORF* vector) {
    glProgramUniform2f(id, glGetUniformLocation(id, name), vector->x, vector->y);
}

void setUniform3f(U32 id, I8* name, F32 x, F32 y, F32 z) {
    glProgramUniform3f(id, glGetUniformLocation(id, name), x, y, z);
}

void setUniform3fP(U32 id, I8* name, VECTORF* vector) {
    glProgramUniform3f(id, glGetUniformLocation(id, name), vector->x, vector->y, vector->z);
}

void setUniform4f(U32 id, I8* name, F32 x, F32 y, F32 z, F32 w) {
    glProgramUniform4f(id, glGetUniformLocation(id, name), x, y, z, w);
}

void setUniform4fP(U32 id, I8* name, VECTORF* vector) {
    glProgramUniform4f(id, glGetUniformLocation(id, name), vector->x, vector->y, vector->z, vector->w);
}

void setUniformMat4(U32 id, I8* name, MATRIX* matrix) {
    F32* values = matrixGetValues(matrix);
    glProgramUniformMatrix4fv(id, glGetUniformLocation(id, name), 1, GL_FALSE, (const F32*)values);
    free(values);
}


