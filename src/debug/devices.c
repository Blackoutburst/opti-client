#include <stdlib.h>

#include "devices/devices.h"

static DEVICE* primaryDevice = NULL;
static DEVICE* secondaryDevice = NULL;

DEVICE* devicePrimary(void) {
    return primaryDevice;
}

DEVICE* deviceSecondary(void) {
    return secondaryDevice;
}

void deviceClean(void) {
}

void deviceInit(void) {
}


