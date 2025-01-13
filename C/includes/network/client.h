#pragma once
#define MAX_BUFFER_SIZE 5000

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    
    void closeConnectionWIN32();
    void connectionSendWIN32(char* buffer, short size);
    void connectionReadWIN32();
    DWORD WINAPI connectionReadLoopWIN32(LPVOID arg);
    void openConnectionWIN32(char* ip, short port);
#else
    #include <pthread.h>
    #include <unistd.h>
    #include <arpa/inet.h>

    void closeConnectionPOSIX();
    void connectionSendPOSIX(char* buffer, short size);
    void connectionReadPOSIX();
    void* connectionReadLoopPOSIX(void* arg);
    void openConnectionPOSIX(char* ip, short port);
#endif

void closeConnection();
void connectionSend(char* buffer, short size);
void connectionRead();
void openConnection(char* ip, short port);
short getPacketSize(char packetID);

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
