#include <stdlib.h>
#include <string.h>
#include "utils/ioUtils.h"
#include "network/client.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <pthread.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

#define MAX_BUFFER_SIZE 5000

static int sockfd = -1;

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

void closeConnection() {
    if (sockfd < 0) return;
    close(sockfd);
    sockfd = -1;
}

void connectionSend(char* buffer, short size) {
    if (sockfd < 0) return;
    send(sockfd, buffer, size, 0);
}

void connectionRead() {
    if (sockfd < 0) return;
    char idBuffer[1];
    if (recv(sockfd, idBuffer, 1, 0) <= 0) {
        println("Data read failed");
        close(sockfd);
        sockfd = -1;
        return;
    }
    short size = getPacketSize(idBuffer[0]);
    if (size <= 0) {
        printf("Invalid packet size: %i\n", size);
        return;
    }
    printf("Packet size: %i\n", size);

    char buffer[MAX_BUFFER_SIZE];
    int totalBytesRead = 0;
    while (totalBytesRead < size) {
        int bytesRead = recv(sockfd, buffer + totalBytesRead, size - totalBytesRead, 0);
        totalBytesRead += bytesRead;

        if (bytesRead <= 0) {
            println("Data read failed");
            close(sockfd);
            sockfd = -1;
            return;
        }
    }
}

void* connectionReadLoop(void* arg) {
    while (sockfd >= 0) {
        connectionRead();
    }

    return NULL;
}

void openConnection(char* ip, short port) {
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
        close(sockfd);
        sockfd = -1;
        return;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        println("Connection failed");
        close(sockfd);
        sockfd = -1;
        return;
    }
    println("Connected successfully!");

    pthread_t thread;
    if (pthread_create(&thread, NULL, connectionReadLoop, NULL)) {
        println("Network thread creation failed");
        return;
    }
}
