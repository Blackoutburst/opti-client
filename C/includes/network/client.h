#pragma once
#include "utils/types.h"
#define MAX_BUFFER_SIZE 5000

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    
    void closeConnectionWIN32();
    void connectionSendWIN32(I8* buffer, I16 size);
    void connectionReadWIN32();
    DWORD WINAPI connectionReadLoopWIN32(LPVOID arg);
    void openConnectionWIN32(I8* ip, I16 port);
#else
    #include <pthread.h>
    #include <unistd.h>
    #include <arpa/inet.h>

    void closeConnectionPOSIX();
    void connectionSendPOSIX(I8* buffer, I16 size);
    void connectionReadPOSIX();
    void* connectionReadLoopPOSIX(void* arg);
    void openConnectionPOSIX(I8* ip, I16 port);
#endif

void closeConnection();
void connectionSend(I8* buffer, I16 size);
void connectionRead();
void openConnection(I8* ip, I16 port);
U16 getPacketSize(I8 packetID);

