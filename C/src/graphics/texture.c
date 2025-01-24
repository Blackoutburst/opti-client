#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include "utils/types.h"
#include "glfw/glfw3.h"
#include "graphics/opengl.h"
#include "utils/math.h"
#include "stb/stb_image.h"

void textureDelete(U32 id) {
    glDeleteTextures(1, &id);
}

U32 textureCreate(const I8* path, U8 flip) {
    U32 id = 0;
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);

    I32 w = 0;
    I32 h = 0;
    I32 comp = 0;

    stbi_set_flip_vertically_on_load(flip);
    
    U8* data = stbi_load(path, &w, &h, &comp, 4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_set_flip_vertically_on_load(0);

    return id;
}
