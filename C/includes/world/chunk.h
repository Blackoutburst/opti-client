typedef struct chunk CHUNK;

struct chunk {
    unsigned int vaoID;
    unsigned int vboID;
    unsigned int eboID;
    unsigned int vertexCount;
    int* position;
    char* blocks;
};

void printChunk(CHUNK* chunk);
int packVertexData(char x, char y, char z, char u, char v, char n);
void generateChunkVAO(CHUNK * chunk, int* mesh);
void cleanChunkMesh(int* mesh);
int* generateChunkMesh(CHUNK* chunk);
CHUNK* createChunk(int* position, char* blocks);
void renderChunk(CHUNK* chunk);
void destroyChunk(CHUNK* chunk);

