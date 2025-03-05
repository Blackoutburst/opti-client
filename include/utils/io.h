#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "utils/types.h"

I64 ioFileSize(FILE* file);
FILE* ioOpenFile(const I8* filePath);
I8* ioReadFile(FILE* file);
