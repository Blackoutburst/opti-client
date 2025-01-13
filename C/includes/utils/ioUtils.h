#pragma once
#include <stdio.h>
#include "utils/types.h"

void println(I8* str);
I64 getFileSize(FILE** file);
I8* readFile(const I8* path);
