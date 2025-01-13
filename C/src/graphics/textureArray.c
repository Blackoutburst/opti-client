#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include "utils/types.h"
#include "GLFW/glfw3.h"
#include "utils/math.h"
#include "stb/stb_image.h"

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include "GL/glew.h"
    #include <GL/gl.h>
#else
    #include <GL/gl.h>
#endif

int createTextureArray(char** files, short filesCount, short fileSize) {
    unsigned int id = 0;
    glGenTextures(1, &id);
    
    glBindTexture(GL_TEXTURE_2D_ARRAY, id);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, fileSize, fileSize, filesCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    int w, h, comp;
    for (int i = 0; i < filesCount; i++) {
        unsigned char* data = stbi_load(files[i], &w, &h, &comp, 4);
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
