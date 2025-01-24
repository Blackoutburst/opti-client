#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <stdlib.h>
#include "utils/types.h"
#include "glfw/glfw3.h"
#include "graphics/opengl.h"
#include "utils/math.h"
#include "stb/stb_image.h"

void textureArrayDelete(U32 id) {
    glDeleteTextures(1, &id);
}

U32 textureArrayCreate(I8** files, I16 filesCount, I16 fileSize) {
    U32 id = 0;
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D_ARRAY, id);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, fileSize, fileSize, filesCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    I32 w = 0;
    I32 h = 0;
    I32 comp = 0;
    for (I32 i = 0; i < filesCount; i++) {
        U8* data = stbi_load(files[i], &w, &h, &comp, 4);
        if (!data) {
            printf("Failed to load image %s\n", files[i]);
            return 0;
        }

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, fileSize, fileSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return id;
}
