#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils/args.h"

static U8 validationLayers = 0;

static I8* args[4] = {
    "-h", "--help",
    "-d", "--debug",
};

void argsParse(I32 argc, I8** argv) {
    for (I32 i = 1; i < argc; i++) {
        if (!strcmp(argv[i], args[0]) || !strcmp(argv[i], args[1])) {
            argsHelp();
        }
        if (!strcmp(argv[i], args[2]) || !strcmp(argv[i], args[3])) {
            argsValidationLayers();
        }
    }
}

U8 argsGetValidationLayers(void) {
    return validationLayers;
}

void argsValidationLayers(void) {
    validationLayers = 1;
}

void argsHelp(void) {
    const I8* help = "==============================================\n"
                    "[Minecraft Wish client]\n\n"
                    "-h, --help:\n"
                    "\tShow this message\n\n"
                    "-d, --debug:"
                    "\tEnable Vulkan validation layers\n\n"
                    "==============================================\n";

    printf("%s", help);
    exit(0);
}
