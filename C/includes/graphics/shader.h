#pragma once

#include <stdlib.h>

#include "graphics/opengl.h"
#include "utils/types.h"
#include "utils/matrix.h"
#include "utils/ioUtils.h"
#include "utils/vectori.h"
#include "utils/vectorf.h"

void deleteShaderProgram(U32 id);
U32 createShaderProgram(U32 vertexshader, U32 fragmentShader);
U32 compileShader(I8* source, U32 type);
void setUniform1i(U32 id, I8* name, I32 x);
void setUniform1i(U32 id, I8* name, I32 x);
void setUniform2i(U32 id, I8* name, I32 x, I32 y);
void setUniform2iP(U32 id, I8* name, VECTORI* vector);
void setUniform3i(U32 id, I8* name, I32 x, I32 y, I32 z);
void setUniform3iP(U32 id, I8* name, VECTORI* vector);
void setUniform4i(U32 id, I8* name, I32 x, I32 y, I32 z, I32 w);
void setUniform4iP(U32 id, I8* name, VECTORI* vector);
void setUniform1f(U32 id, I8* name, F32 x);
void setUniform2f(U32 id, I8* name, F32 x, F32 y);
void setUniform2fP(U32 id, I8* name, VECTORF* vector);
void setUniform3f(U32 id, I8* name, F32 x, F32 y, F32 z);
void setUniform3fP(U32 id, I8* name, VECTORF* vector);
void setUniform4f(U32 id, I8* name, F32 x, F32 y, F32 z, F32 w);
void setUniform4fP(U32 id, I8* name, VECTORF* vector);
void setUniformMat4(U32 id, I8* name, MATRIX* matrix);
