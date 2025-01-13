#include <stdio.h>
#include <stdlib.h>
#include "utils/types.h"

void println(I8* str) {
    printf("%s\n", str);
}

I64 getFileSize(FILE** file) {
    fseek(*file, 0, SEEK_END);
    I64 fileSize = ftell(*file);
    rewind(*file);

    return fileSize;
}

I8* readFile(const I8* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        println("Couldn't find file");
        return NULL;
    }

    size_t fileSize = getFileSize(&file);
    if (!fileSize) {
        println("File size error");
        fclose(file);
        return NULL;
    }

    I8* buffer = (I8*)malloc(fileSize + 1);
    if (!buffer) {
        println("File allocation failed");
        fclose(file);
        return NULL;
    }

    size_t data = fread(buffer, 1, fileSize, file);
    if (data != fileSize) {
        println("Reading file failed");
        fclose(file);
        return NULL;
    }
    
    buffer[fileSize] = 0x00;
    fclose(file);

    return buffer;
}

