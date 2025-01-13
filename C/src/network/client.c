#include <stdlib.h>
#include <string.h>
#include "utils/ioUtils.h"
#include "network/client.h"

#if defined(_WIN32) || defined(_WIN64)
    static SOCKET sockfd = INVALID_SOCKET;
#else
    static int sockfd = -1;
#endif

short getPacketSize(char packetID) {
    switch (packetID) {
        case PACKET_IDENTIFICATION:
            return 4;
        case PACKET_ADD_ENTITY:
            return 88;
        case PACKET_REMOVE_ENTITY:
            return 4;
        case PACKET_UPDATE_ENTITY:
            return 24;
        case PACKET_SEND_CHUNK:
            return 4108;
        case PACKET_SEND_MONOTYPE_CHUNK:
            return 13;
        case PACKET_CHAT:
            return 4096;
        case PACKET_UPDATE_ENTITY_METADATA:
            return 68;
        default:
            return -1;
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
    void connectionSendWIN32(char* buffer, short size) {
        if (sockfd == INVALID_SOCKET) return;
        send(sockfd, buffer, size, 0);
    }
#else
    void connectionSendPOSIX(char* buffer, short size) {
        if (sockfd < 0) return;
        send(sockfd, buffer, size, 0);
    }
#endif

void connectionSend(char* buffer, short size) {
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
        char idBuffer[1];
        if (recv(sockfd, idBuffer, 1, 0) <= 0) {
            println("Data read failed");
            closeConnection();
            return;
        }

        short size = getPacketSize(idBuffer[0]);
        if (size <= 0) {
            printf("Invalid packet size: %i\n", size);
            return;
        }

        char buffer[MAX_BUFFER_SIZE];
        int totalBytesRead = 0;
        while (totalBytesRead < size) {
            int bytesRead = recv(sockfd, buffer + totalBytesRead, size - totalBytesRead, 0);
            if (bytesRead <= 0) {
                println("Data read failed");
                closeConnection();
                return;
            }
            totalBytesRead += bytesRead;
        }
        
    }
#else
    void connectionReadPOSIX() {
        if (sockfd < 0) return;
        char idBuffer[1];
        if (recv(sockfd, idBuffer, 1, 0) <= 0) {
            println("Data read failed");
            closeConnection();
            return;
        }
        short size = getPacketSize(idBuffer[0]);
        if (size <= 0) {
            printf("Invalid packet size: %i\n", size);
            return;
        }

        char buffer[MAX_BUFFER_SIZE];
        int totalBytesRead = 0;
        while (totalBytesRead < size) {
            int bytesRead = recv(sockfd, buffer + totalBytesRead, size - totalBytesRead, 0);
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
    void openConnectionWIN32(char* ip, short port) {
        struct sockaddr_in server_addr;
        
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
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
    void openConnectionPOSIX(char* ip, short port) {
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

void openConnection(char* ip, short port) {
    #if defined(_WIN32) || defined(_WIN64)
        openConnectionWIN32(ip, port);
    #else
        openConnectionPOSIX(ip, port);
    #endif
}
