#include <stdlib.h>
#include <string.h>
#include "utils/ioUtils.h"
#include "network/client.h"
#include "network/packet.h"
#include "utils/types.h"

#if defined(_WIN32) || defined(_WIN64)
    static SOCKET sockfd = INVALID_SOCKET;
#else
    static I32 sockfd = -1;
#endif

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

        U8 dataBuffer[MAX_BUFFER_SIZE];
        U8* bufferPtr = dataBuffer;
        U32 totalBytesRead = 0;
        while (totalBytesRead < size) {
            I32 bytesRead = recv(sockfd, (I8*)(dataBuffer + totalBytesRead), size - totalBytesRead, 0);
            if (bytesRead <= 0) {
                println("Data read failed");
                closeConnection();
                return;
            }
            totalBytesRead += bytesRead;
        }
        // TODO: move this shit away
        if(packetId == CLIENT_PACKET_UPDATE_ENTITY) {
            U32 entityId = getU32((U8**)&bufferPtr);
            F32 x = getF32((U8**)&bufferPtr);
            F32 y = getF32((U8**)&bufferPtr);
            F32 z = getF32((U8**)&bufferPtr);
            F32 yaw = getF32((U8**)&bufferPtr);
            F32 pitch = getF32((U8**)&bufferPtr);

            printf("id: %i, x: %f, y: %f, z: %f, yaw: %f, pitch: %f\n", entityId, x, y, z, yaw, pitch);
        }
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

        U8 dataBuffer[MAX_BUFFER_SIZE];
        //U8* bufferPtr = dataBuffer;
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

    #if defined(_WIN32) || defined(_WIN64)
        openConnectionWIN32(ip, port);
    #else
        openConnectionPOSIX(ip, port);
    #endif
}
