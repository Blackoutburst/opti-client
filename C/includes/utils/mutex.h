#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
#else
    #include <unistd.h>
    #include <pthread.h>
#endif

void mutexCreate(void* mutex);
void mutexDestroy(void* mutex);
void mutexLock(void* mutex);
void mutexUnlock(void* mutex);
