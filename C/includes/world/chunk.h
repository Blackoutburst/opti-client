typedef struct chunk CHUNK;

struct chunk {
    int vaoID;
    int vboID;
    int eboID;
    unsigned short indexCount;
    int* position;
    char* blocks;
};

CHUNK createChunk();
void printChunk(CHUNK chunk);
