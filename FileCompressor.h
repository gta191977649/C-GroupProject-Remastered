/* Defines */
#define MAX_ALPHABIT_LEN 256
#define FILE_BUF_SIZE 8192 
/* the size for the file buffer (used in the process) */

/*Struct for huffman tree node*/
typedef struct Node
{
    char character;     /*character*/
    int frequency;      /*frequency*/
    struct Node *left;  /*left child*/
    struct Node *right; /*rigth child*/
} node_t;

/* Struct for file info */
typedef struct FileInfo
{
    node_t *huffmanTree;
    unsigned int characterCount;
    unsigned int byteTotal;
    char *dictionary[MAX_ALPHABIT_LEN];
    int numberOfEachCharacters[MAX_ALPHABIT_LEN];
} fileinfo_t;
/* Function prototypes */
fileinfo_t *mallocFileInfo();

/******************************************************************************
    prototypes
******************************************************************************/
void freeFileInfo(fileinfo_t *);

void compressorProcessFile(fileinfo_t *, const char *, const char *);
void compressorGenerateStatistic(fileinfo_t *, const char *);
void compressorCreaterHuffmanTree(fileinfo_t *);
void compressorProcessFile(fileinfo_t *fileInfo, const char *inputFileName, 
    const char *outputFileName);
void compressorGenerateStatistic(fileinfo_t *fileInfo, 
    const char *inputFileName);
void decompressorProcessFile(fileinfo_t *fileInfop, 
    const char *inputFileNamep, const char *outputFileNamep);

void compressorGenerateLookupTable(fileinfo_t *fileInfo);
void compressorWriteToFile(fileinfo_t *fcs, const char *inFileName, 
    const char *outFileName);
void printLookuptable(fileinfo_t *fileInfo);
