#pragma once
I32 compileShader(const I8* source, U32 type);
I32 createShaderProgram(I32 vertexshader, I32 fragmentShader);
void setUniform1i(I32 id, I8* name, I32 x);
void setUniform1i(I32 id, I8* name, I32 x);
void setUniform2i(I32 id, I8* name, I32 x, I32 y);
void setUniform2iP(I32 id, I8* name, I32* x);
void setUniform3i(I32 id, I8* name, I32 x, I32 y, I32 z);
void setUniform3iP(I32 id, I8* name, I32* x);
void setUniform4i(I32 id, I8* name, I32 x, I32 y, I32 z, I32 w);
void setUniform4iP(I32 id, I8* name, I32* x);
void setUniform1f(I32 id, I8* name, F32 x);
void setUniform2f(I32 id, I8* name, F32 x, F32 y);
void setUniform2fP(I32 id, I8* name, F32* x);
void setUniform3f(I32 id, I8* name, F32 x, F32 y, F32 z);
void setUniform3fP(I32 id, I8* name, F32* x);
void setUniform4f(I32 id, I8* name, F32 x, F32 y, F32 z, F32 w);
void setUniform4fP(I32 id, I8* name, F32* x);
void setUniformMat4(I32 id, I8* name, const F32* matrix);
