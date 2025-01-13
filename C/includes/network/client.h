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
I16 getPacketSize(I8 packetID);

enum Packets {
    PACKET_IDENTIFICATION,
    PACKET_ADD_ENTITY,
    PACKET_REMOVE_ENTITY,
    PACKET_UPDATE_ENTITY,
    PACKET_SEND_CHUNK,
    PACKET_SEND_MONOTYPE_CHUNK,
    PACKET_CHAT,
    PACKET_UPDATE_ENTITY_METADATA,
};
