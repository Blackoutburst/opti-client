#pragma once
int compileShader(const char* source, unsigned int type);
int createShaderProgram(int vertexshader, int fragmentShader);
void setUniform1i(int id, char* name, int x);
void setUniform1i(int id, char* name, int x);
void setUniform2i(int id, char* name, int x, int y);
void setUniform2iP(int id, char* name, int* x);
void setUniform3i(int id, char* name, int x, int y, int z);
void setUniform3iP(int id, char* name, int* x);
void setUniform4i(int id, char* name, int x, int y, int z, int w);
void setUniform4iP(int id, char* name, int* x);
void setUniform1f(int id, char* name, float x);
void setUniform2f(int id, char* name, float x, float y);
void setUniform2fP(int id, char* name, float* x);
void setUniform3f(int id, char* name, float x, float y, float z);
void setUniform3fP(int id, char* name, float* x);
void setUniform4f(int id, char* name, float x, float y, float z, float w);
void setUniform4fP(int id, char* name, float* x);
void setUniformMat4(int id, char* name, const float* matrix);
