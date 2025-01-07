typedef struct chunk CHUNK;

struct chunk {
    unsigned int vaoID;
    unsigned int vboID;
    unsigned int eboID;
    unsigned int indexCount;
    int* position;
    char* blocks;
};

void printChunk(CHUNK* chunk);
int** generateChunkMesh(CHUNK* chunk);
CHUNK* createChunk(int* position, char* blocks);
void destroyChunk(CHUNK* chunk);
