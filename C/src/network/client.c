#include <stdlib.h>
#include <string.h>
#include "utils/ioUtils.h"
#include "network/client.h"
#include "network/packet.h"
#include "network/decoder.h"
#include "utils/types.h"

#if defined(_WIN32) || defined(_WIN64)
    static SOCKET sockfd = INVALID_SOCKET;
#else
    static I32 sockfd = -1;
#endif

static NET_QUEUE* networkQueue = NULL;

void networkQueueCleanElement(U16 index) {
    if (networkQueue == NULL) return;
    NET_QUEUE_ELEM* element = networkQueue->elements[index];
    if (element == NULL) return;
    free(element->buffer);
    free(element);
    networkQueue->elements[index] = NULL;
}

void networkQueuePush(void (*function)(U8*), U8* buffer) {
    if (networkQueue == NULL) return;
    if (networkQueue->elements[networkQueue->pushIndex] != NULL) {
        networkQueueCleanElement(networkQueue->pushIndex);
    }
    networkQueue->elements[networkQueue->pushIndex] = malloc(sizeof(NET_QUEUE_ELEM));

    networkQueue->elements[networkQueue->pushIndex]->function = function;
    networkQueue->elements[networkQueue->pushIndex]->buffer = buffer;
    networkQueue->elements[networkQueue->pushIndex]->id = networkQueue->pushIndex;

    networkQueue->pushIndex++;
    if (networkQueue->pushIndex >= networkQueue->size) {
        networkQueue->pushIndex = 0;
    }
}

U8 networkQueuePop(NET_QUEUE_ELEM** element) {
    if (networkQueue == NULL) return 0;
    NET_QUEUE_ELEM* elem = networkQueue->elements[networkQueue->popIndex];
    if (elem == NULL) return 0;

    networkQueue->popIndex++;
    if (networkQueue->popIndex >= networkQueue->size) {
        networkQueue->popIndex = 0;
    }

    *element = elem;
    return 1;
}

void networkQueueInit() {
    if (networkQueue != NULL) return;
    NET_QUEUE* queue = malloc(sizeof(NET_QUEUE));
    queue->size = QUEUE_SIZE;
    queue->pushIndex = 0;
    queue->popIndex = 0;
    queue->elements = malloc(sizeof(NET_QUEUE_ELEM*) * QUEUE_SIZE);
    for (int i = 0; i < QUEUE_SIZE; i++) queue->elements[i] = NULL;
    networkQueue = queue;
}

void networkQueueClean() {
    if (networkQueue == NULL) return;
    for (int i = 0; i < QUEUE_SIZE; i++) networkQueueCleanElement(i);
    free(networkQueue->elements);
    free(networkQueue);
    networkQueue = NULL;
}

void* getPacketfunction(I8 packetID) {
    switch (packetID) {
        case CLIENT_PACKET_IDENTIFICATION:
            return &decodePacketIdentification;
        case CLIENT_PACKET_ADD_ENTITY:
            return &decodePacketAddEntity;
        case CLIENT_PACKET_REMOVE_ENTITY:
            return &decodePacketRemoveEntity;
        case CLIENT_PACKET_UPDATE_ENTITY:
            return &decodePacketUpdateEntity;
        case CLIENT_PACKET_SEND_CHUNK:
            return &decodePacketSendChunk;
        case CLIENT_PACKET_SEND_MONOTYPE_CHUNK:
            return &decodePacketSendMonotypeChunk;
        case CLIENT_PACKET_CHAT:
            return &decodePacketChat;
        case CLIENT_PACKET_UPDATE_ENTITY_METADATA:
            return &decodePacketUpdateEntityMetadata;
        default:
            return NULL;
    }
}

U16 getPacketSize(I8 packetID) {
    switch (packetID) {
        case CLIENT_PACKET_IDENTIFICATION:
            return 4;
        case CLIENT_PACKET_ADD_ENTITY:
            return 88;
        case CLIENT_PACKET_REMOVE_ENTITY:
            return 4;
        case CLIENT_PACKET_UPDATE_ENTITY:
            return 24;
        case CLIENT_PACKET_SEND_CHUNK:
            return 4108;
        case CLIENT_PACKET_SEND_MONOTYPE_CHUNK:
            return 13;
        case CLIENT_PACKET_CHAT:
            return 4096;
        case CLIENT_PACKET_UPDATE_ENTITY_METADATA:
            return 68;
        default:
            return 0;
    }
}

///// CLOSE /////

#if defined(_WIN32) || defined(_WIN64)
    void closeConnectionWIN32() {
        if (sockfd == INVALID_SOCKET) return;
        closesocket(sockfd);
        sockfd = INVALID_SOCKET;
        WSACleanup();
    }
#else
    void closeConnectionPOSIX() {
        if (sockfd < 0) return;
        close(sockfd);
        sockfd = -1;
    }
#endif

void closeConnection() {
    #if defined(_WIN32) || defined(_WIN64)
        closeConnectionWIN32();
    #else
        closeConnectionPOSIX();
    #endif
    networkQueueClean();
}

///// SEND /////

#if defined(_WIN32) || defined(_WIN64)
    void connectionSendWIN32(I8* buffer, I16 size) {
        if (sockfd == INVALID_SOCKET) return;
        send(sockfd, buffer, size, 0);
    }
#else
    void connectionSendPOSIX(I8* buffer, I16 size) {
        if (sockfd < 0) return;
        send(sockfd, buffer, size, 0);
    }
#endif

void connectionSend(I8* buffer, I16 size) {
    #if defined(_WIN32) || defined(_WIN64)
        connectionSendWIN32(buffer, size);
    #else
        connectionSendPOSIX(buffer, size);
    #endif
}

///// READ /////

#if defined(_WIN32) || defined(_WIN64)
    void connectionReadWIN32() {
        if (sockfd == INVALID_SOCKET) return;
        I8 packetId;
        if (recv(sockfd, &packetId, 1, 0) <= 0) {
            println("Data read failed");
            closeConnection();
            return;
        }

        U16 size = getPacketSize(packetId);
        if (size <= 0) {
            printf("Invalid packet size: %i\n", size);
            return;
        }

        U8* dataBuffer = malloc(size);
        U32 totalBytesRead = 0;
        while (totalBytesRead < size) {
            I32 bytesRead = recv(sockfd, (I8*)(dataBuffer + totalBytesRead), size - totalBytesRead, 0);
            totalBytesRead += bytesRead;

            if (bytesRead <= 0) {
                println("Data read failed");
                closeConnection();
                return;
            }
        }

        void* func = getPacketfunction(packetId);
        networkQueuePush(func, dataBuffer);
    }
#else
    void connectionReadPOSIX() {
        if (sockfd < 0) return;
        I8 packetId;
        if (recv(sockfd, &packetId, 1, 0) <= 0) {
            println("Data read failed");
            closeConnection();
            return;
        }
        U16 size = getPacketSize(packetId);
        if (size <= 0) {
            printf("Invalid packet size: %i\n", size);
            return;
        }

        U8* dataBuffer = malloc(size);
        U32 totalBytesRead = 0;
        while (totalBytesRead < size) {
            I32 bytesRead = recv(sockfd, dataBuffer + totalBytesRead, size - totalBytesRead, 0);
            totalBytesRead += bytesRead;

            if (bytesRead <= 0) {
                println("Data read failed");
                closeConnection();
                return;
            }
        }

        void* func = getPacketfunction(packetId);
        networkQueuePush(func, dataBuffer);
    }
#endif

void connectionRead() {
    #if defined(_WIN32) || defined(_WIN64)
        connectionReadWIN32();
    #else
        connectionReadPOSIX();
    #endif
}

///// READ LOOP /////

#if defined(_WIN32) || defined(_WIN64)
    DWORD WINAPI connectionReadLoopWIN32(LPVOID arg) {
        while (sockfd != INVALID_SOCKET) {
            connectionRead();
        }

        return 0;
    }
#else
    void* connectionReadLoopPOSIX(void* arg) {
        while (sockfd >= 0) {
            connectionRead();
        }

        return NULL;
    }
#endif

///// OPEN /////

#if defined(_WIN32) || defined(_WIN64)
    void openConnectionWIN32(I8* ip, I16 port) {
        struct sockaddr_in server_addr;

        WSADATA wsaData;
        I32 result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            println("WSA startup failed");
            return;
        }

        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == INVALID_SOCKET) {
            printf("Socket creation failed, error code: %d\n", WSAGetLastError());
            WSACleanup();
            return;
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family      = AF_INET;
        server_addr.sin_port        = htons(port);

        if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
            println("Invalid server address");
            closeConnection();
            return;
        }

        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            println("Connection failed");
            closeConnection();
            return;
        }

        println("Connected successfully!");

        HANDLE thread = CreateThread(NULL, 0, connectionReadLoopWIN32, NULL, 0, NULL);
        if (thread == NULL) {
            println("Network thread creation failed");
            return;
        }

        CloseHandle(thread);
    }
#else
    void openConnectionPOSIX(I8* ip, I16 port) {
        struct sockaddr_in server_addr;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            println("Socket creation failed");
            return;
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
            println("Invalid server address");
            closeConnection();
            return;
        }

        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            println("Connection failed");
            closeConnection();
            return;
        }
        println("Connected successfully!");

        pthread_t thread;
        if (pthread_create(&thread, NULL, connectionReadLoopPOSIX, NULL)) {
            println("Network thread creation failed");
            return;
        }
    }
#endif

void openConnection(I8* ip, I16 port) {
    networkQueueInit();
    #if defined(_WIN32) || defined(_WIN64)
        openConnectionWIN32(ip, port);
    #else
        openConnectionPOSIX(ip, port);
    #endif
}
