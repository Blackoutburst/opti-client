#include "utils/io.h"
#include "utils/logger.h"


I64 ioFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    I64 fileSize = ftell(file);
    rewind(file);

    return fileSize;
}

FILE* ioOpenFile(const I8* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (!file) {
        logW("Couldn't find file");
        return NULL;
    }

    return file;
}

I8* ioReadFile(FILE* file) {
    I64 fileSize = ioFileSize(file);
    if (!fileSize) {
        logW("File size error");
        fclose(file);
        return NULL;
    }

    I8* buffer = (I8*)malloc(fileSize + 1);
    if (!buffer) {
        logW("File allocation failed");
        fclose(file);
        return NULL;
    }

    I64 data = fread(buffer, 1, fileSize, file);
    if (data != fileSize) {
        logW("Reading file failed");
        fclose(file);
        return NULL;
    }
    
    buffer[fileSize] = 0x00;
    fclose(file);

    return buffer;
}

