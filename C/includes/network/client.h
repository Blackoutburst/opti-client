void closeConnection();
void connectionSend(char* buffer, short size);
void connectionRead();
void* connectionReadLoop(void* arg);
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
