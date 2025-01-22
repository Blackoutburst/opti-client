#include <stdlib.h>
#include <string.h>
#include "utils/ioUtils.h"
#include "network/client.h"
#include "network/packet.h"
#include "network/decoder.h"
#include "network/networkQueue.h"
#include "utils/types.h"

#if defined(_WIN32) || defined(_WIN64)
    static SOCKET sockfd = INVALID_SOCKET;
#else
    static I32 sockfd = -1;
#endif

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
    void connectionSendWIN32(U8* buffer, I16 size) {
        if (sockfd == INVALID_SOCKET) return;
        send(sockfd, (I8*)buffer, size, 0);
    }
#else
    void connectionSendPOSIX(U8* buffer, I16 size) {
        if (sockfd < 0) return;
        send(sockfd, buffer, size, 0);
    }
#endif

void connectionSend(U8* buffer, I16 size) {
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

        U16 size = getClientPacketSize(packetId);
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

        void* func = getClientPacketfunction(packetId);
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
        U16 size = getClientPacketSize(packetId);
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
        void* func = getClientPacketfunction(packetId);
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
