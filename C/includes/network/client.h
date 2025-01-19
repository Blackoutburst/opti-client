#pragma once
#include "utils/types.h"
#define MAX_BUFFER_SIZE 5000
#define QUEUE_SIZE 4096

typedef struct networkQueue NET_QUEUE;
typedef struct networkQueueElement NET_QUEUE_ELEM;

struct networkQueueElement {
    U16 id;
    void (*function)(U8*);
    U8* buffer;
};

struct networkQueue {
    U16 size;
    U16 pushIndex;
    U16 popIndex;
    NET_QUEUE_ELEM** elements;
};

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>

    void closeConnectionWIN32();
    void connectionSendWIN32(U8* buffer, I16 size);
    void connectionReadWIN32();
    DWORD WINAPI connectionReadLoopWIN32(LPVOID arg);
    void openConnectionWIN32(I8* ip, I16 port);
#else
    #include <pthread.h>
    #include <unistd.h>
    #include <arpa/inet.h>

    void closeConnectionPOSIX();
    void connectionSendPOSIX(U8* buffer, I16 size);
    void connectionReadPOSIX();
    void* connectionReadLoopPOSIX(void* arg);
    void openConnectionPOSIX(I8* ip, I16 port);
#endif

void networkQueueInit();
void networkQueueClean();
void networkQueueCleanElement(U16 index);
void networkQueuePush(void (*function)(U8*), U8* buffer);
U8 networkQueuePop(NET_QUEUE_ELEM** element);

void closeConnection();
void connectionSend(U8* buffer, I16 size);
void connectionRead();
void openConnection(I8* ip, I16 port);

void* getPacketfunction(I8 packetID);
U16 getPacketSize(I8 packetID);

