#include <stdio.h>
#include <stdlib.h>

void println(char* str) {
    printf("%s\n", str);
}

long getFileSize(FILE** file) {
    fseek(*file, 0, SEEK_END);
    long fileSize = ftell(*file);
    rewind(*file);

    return fileSize;
}

char* readFile(const char* path) {
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

    char* buffer = (char*)malloc(fileSize + 1);
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

