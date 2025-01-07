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
int packVertexData(int x, int y, int z, int u, int v, int n);
void generateChunkVAO(CHUNK * chunk, int** mesh);
void cleanChunkMesh(int** mesh);
int** generateChunkMesh(CHUNK* chunk);
CHUNK* createChunk(int* position, char* blocks);
void destroyChunk(CHUNK* chunk);

