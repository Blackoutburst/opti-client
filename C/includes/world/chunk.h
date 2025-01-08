typedef struct chunk CHUNK;
typedef struct chunkMesh CHUNK_MESH;

struct chunk {
    unsigned int vaoID;
    unsigned int vboID;
    unsigned int eboID;
    unsigned int indexCount;
    int* position;
    char* blocks;
};

struct chunkMesh {
    int* vertices;
    int* indices;
    int vertexCount;
    int indexCount;
};

void printChunk(CHUNK* chunk);
int packVertexData(char x, char y, char z, char u, char v, char n);
void generateChunkVAO(CHUNK * chunk, CHUNK_MESH* mesh);
void cleanChunkMesh(CHUNK_MESH* mesh);
CHUNK_MESH* generateChunkMesh(CHUNK* chunk);
CHUNK* createChunk(int* position, char* blocks);
void renderChunk(CHUNK* chunk);
void destroyChunk(CHUNK* chunk);

