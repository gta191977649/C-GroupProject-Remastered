
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FileCompressor.h"
#include "PriorityQueue.h"
#include "FileHandle.h"
#include "Common.h"

node_t *newHuffmanTree(char character, int frequency)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->character = character;
    node->frequency = frequency;
    return node;
}
void freeHuffmanTree(node_t *root)
{
    if (root)
    {
        freeHuffmanTree(root->left);
        freeHuffmanTree(root->right);
        free(root);
    }
}

fileinfo_t *mallocFileInfo()
{
    fileinfo_t *fileInfo = (fileinfo_t *)malloc(sizeof(fileinfo_t));
    fileInfo->characterCount = 0;
    fileInfo->byteTotal = 0;
    memset(fileInfo->numberOfEachCharacters, 0, sizeof(fileInfo->numberOfEachCharacters));
    memset(fileInfo->dictionary, 0, sizeof(fileInfo->dictionary));
    fileInfo->huffmanTree = NULL;
    return fileInfo;
}
void freeFileInfo(fileinfo_t *fileInfo)
{
    int i;
    if (fileInfo)
    {
        if (fileInfo->huffmanTree)
        {
            freeHuffmanTree(fileInfo->huffmanTree);
        }
        for (i = 0; i < MAX_CHARS; ++i)
        {
            free(fileInfo->dictionary[i]);
        }
        free(fileInfo);
    }
}

/*Helepr functions for compression */
void compressorGenerateStatistic(fileinfo_t *fileInfo, const char *inputFileName)
{
    int result, i;
    unsigned char fileBuffer[FILE_BUF_SIZE];
    FILE *filep = fopen(inputFileName, "rb");
    if (!filep)
    {
        fprintf(stderr, "Error Cannot open file : "GREEN"%s"WHITE"\n", inputFileName);
        return;
    }
    while ((result = fread(fileBuffer, 1, FILE_BUF_SIZE, filep)) > 0)
    {
        fileInfo->byteTotal += result;
        for (i = 0; i < result; ++i)
        {
            if (fileInfo->numberOfEachCharacters[fileBuffer[i]] == 0)
            {
                fileInfo->characterCount++;
            }
            fileInfo->numberOfEachCharacters[fileBuffer[i]]++;
        }
    }
    fclose(filep);
}

/* Generate a new huffman treee from the fileinfo */
void compressorCreaterHuffmanTree(fileinfo_t *fileInfo)
{

    int i, count;
    node_t *huffmanTree, *parent, *left, *right;

    map_t *kv, *kv1, *kv2;
    priorityqueue_t *pqp;
    pqp = priorityQueueNew(PRIORITY_MIN);

    for (i = 0; i < MAX_ALPHABIT_LEN; ++i)
    {
        if (fileInfo->numberOfEachCharacters[i])
        {
            huffmanTree = newHuffmanTree((char)i, fileInfo->numberOfEachCharacters[i]);
            kv = newMap(fileInfo->numberOfEachCharacters[i], huffmanTree);
            priorityQueueEnqueue(pqp, kv);
        }
    }

    if (debugMode)
        fprintf(stdout, "the number of haffuman leaf is %d\n", priorityQueueSize(pqp));

    while (!priorityQueueEmpty(pqp))
    {
        if(debugMode)
            fprintf(stdout, "[Debug]: Quene Size: %d\n", priorityQueueSize(pqp));
        kv1 = priorityQueueDequeue(pqp);
        kv2 = priorityQueueDequeue(pqp);
        if (kv2 == NULL)
        {
            fileInfo->huffmanTree = kv1->value;
            mapValueFree(kv1, NULL);
        }
        else
        {
            left = (node_t *)kv1->value;
            right = (node_t *)kv2->value;
            count = left->frequency + right->frequency;
            mapValueFree(kv1, NULL);
            mapValueFree(kv2, NULL);
            parent = newHuffmanTree(0, count);
            parent->left = left;
            parent->right = right;
            kv = newMap(count, parent);
            priorityQueueEnqueue(pqp, kv);
        }
    }

    priorityQueueFree(pqp, NULL);
}

/* Using recursiong to generate lookupTable */
void generateLookupTable(node_t *huffmanTreep, char *dictionary[], char path[], int depth)
{
    char *code = NULL;
    if (huffmanTreep)
    {
        if (huffmanTreep->left == NULL && huffmanTreep->right == NULL)
        {
            code = (char *)malloc(sizeof(char) * (depth + 1));
            memset(code, 0, sizeof(char) * (depth + 1));
            memcpy(code, path, depth);
            dictionary[(unsigned char)huffmanTreep->character] = code;
        }

        if (huffmanTreep->left)
        {
            path[depth] = '0';
            generateLookupTable(huffmanTreep->left, dictionary, path, depth + 1);
        }

        if (huffmanTreep->right)
        {
            path[depth] = '1';
            generateLookupTable(huffmanTreep->right, dictionary, path, depth + 1);
        }
    }
}

/* Generate lookupTable */
void compressorGenerateLookupTable(fileinfo_t *fileInfo)
{
    char path[32];
    generateLookupTable(fileInfo->huffmanTree, fileInfo->dictionary, path, 0);
    /* debug mode */
    if (debugMode)
        printLookuptable(fileInfo);
}

/* Debug funtion for lookuptable */

void printLookuptable(fileinfo_t *fileInfo)
{
    printf("Debug for generated lookup table:\n");
    int i;
    for (i = 0; i < MAX_ALPHABIT_LEN; ++i)
    {
        if (fileInfo->dictionary[i] != NULL)
        {
            fprintf(stdout, "%c:%s\n", i, fileInfo->dictionary[i]);
        }
    }
}

/* Write to the file */

void compressorWriteLookUpTableToFile(fileinfo_t *fcs, FILE *pf)
{
    int i;
    fprintf(pf, "%d\n", fcs->characterCount);
    for (i = 0; i < MAX_CHARS; ++i)
    {
        if (fcs->numberOfEachCharacters[i] != 0)
        {
            fprintf(pf, "%d %d\n", i, fcs->numberOfEachCharacters[i]);
        }
    }
}
void compressorWriteToFile(fileinfo_t *fcs, const char *inFileName, const char *outFileName)
{
    int i, j, ret;

    char *dictEntry, len;
    unsigned int bytes;
    char bitBuf;
    int bitPos;

    unsigned char inBuf[FILE_BUF_SIZE];
    FILE *pfIn, *pfOut;

    pfIn = fopen(inFileName, "rb");
    if (!pfIn)
    {
        fprintf(stderr, "can't open file: "GREEN"%s"WHITE"\n", inFileName);
        return;
    }
    pfOut = fopen(outFileName, "wb");
    if (!pfOut)
    {
        fclose(pfIn);
        fprintf(stderr, "can't open file: "GREEN"%s"WHITE"\n", outFileName);
        return;
    }

    compressorWriteLookUpTableToFile(fcs, pfOut);

    bitBuf = 0x00;
    bitPos = 0;
    bytes = 0;
    while ((ret = fread(inBuf, 1, FILE_BUF_SIZE, pfIn)) > 0)
    {
        for (i = 0; i < ret; ++i)
        {
            len = strlen(fcs->dictionary[inBuf[i]]);
            dictEntry = fcs->dictionary[inBuf[i]];
            /*printf("%s\n", dictEntry);*/
            for (j = 0; j < len; ++j)
            {
                if (dictEntry[j] == '1')
                {
                    bitBuf |= mask[bitPos++];
                }
                else
                {
                    bitPos++;
                }

                if (bitPos == BITS_PER_CHAR)
                {
                    fwrite(&bitBuf, 1, sizeof(bitBuf), pfOut);
                    bitBuf = 0x00;
                    bitPos = 0;

                    bytes++;
                }
            }
        }
    }
    if (bitPos != 0)
    {
        fwrite(&bitBuf, 1, sizeof(bitBuf), pfOut);
        bytes++;
    }

    fclose(pfIn);
    fclose(pfOut);
    printf("file compress ssessful, compression rate: %f%%\n", (fcs->byteTotal - bytes) * 100.0 / fcs->byteTotal);
}
void compressorReadStatistic(fileinfo_t *FileInfop, FILE *filep)
{
    int i, charsCount = 0;
    int ch;
    int num;
    fscanf(filep, "%d\n", &charsCount);
    FileInfop->characterCount = charsCount;

    for (i = 0; i < charsCount; ++i)
    {
        fscanf(filep, "%d %d\n", &ch, &num);
        FileInfop->numberOfEachCharacters[(unsigned int)ch] = num;
        FileInfop->byteTotal += num;
    }
}

void decompressorWriteToFile(fileinfo_t *fileInfo, FILE *fileInputp, const char *outPutFileName)
{
    int i, j, ret;
    unsigned char ch;
    node_t *htn;
    unsigned char buf[FILE_BUF_SIZE];
    /*unsigned char bitCode;
    int bitPos;*/
    FILE *pfOut;

    pfOut = fopen(outPutFileName, "wb");
    if (!pfOut)
    {
        fprintf(stderr, "can't open file: "GREEN"%s"WHITE"\n", outPutFileName);
        return;
    }
    htn = fileInfo->huffmanTree;
    /*bitCode = 0x00;
    bitPos = 0;*/
    while ((ret = fread(buf, 1, FILE_BUF_SIZE, fileInputp)) > 0)
    {
        for (i = 0; i < ret; ++i)
        {
            ch = buf[i];

            for (j = 0; j < BITS_PER_CHAR; ++j)
            {
                if (ch & mask[j])
                {
                    htn = htn->right;
                }
                else
                {
                    htn = htn->left;
                }
                if (htn->left == NULL && htn->right == NULL) /*leaf*/
                {
                    if (fileInfo->byteTotal > 0)
                    {
                        fwrite(&htn->character, 1, sizeof(char), pfOut);
                        fileInfo->byteTotal--;
                    }
                    htn = fileInfo->huffmanTree;
                }
            }
        }
    }
    fclose(pfOut);
}

void compressorProcessFile(fileinfo_t *fileInfo, const char *inputFileName, const char *outputFileName)
{
    if (debugMode)
        fprintf(stdout, "[Debug]: To compress file: "GREEN"%s"WHITE" ...\n", inputFileName);
    compressorGenerateStatistic(fileInfo, inputFileName);
    compressorCreaterHuffmanTree(fileInfo);
    compressorGenerateLookupTable(fileInfo);
    compressorWriteToFile(fileInfo, inputFileName, outputFileName);

    fprintf(stdout, "The compressed of file: "GREEN"%s"WHITE" stored as "GREEN"%s"WHITE"\n", inputFileName, outputFileName);
}

/* Decomress the file */
void decompressorProcessFile(fileinfo_t *fileInfop, const char *inputFileNamep, const char *outputFileNamep)
{

    FILE *fileInp;
    if (debugMode)
        fprintf(stdout, "[Debug]: To decompress file: "GREEN"%s"WHITE" ...\n", inputFileNamep);
    fileInp = fopen(inputFileNamep, "rb");
    if (!fileInp)
    {
        fprintf(stderr, "error cannot open file: "GREEN"%s"WHITE"\n", inputFileNamep);
        return;
    }
    compressorReadStatistic(fileInfop, fileInp);
    compressorCreaterHuffmanTree(fileInfop);
    compressorGenerateLookupTable(fileInfop);
    decompressorWriteToFile(fileInfop, fileInp, outputFileNamep);
    fclose(fileInp);
    fprintf(stdout, "The decompressed of file: "GREEN"%s"WHITE" stored at "GREEN"%s"WHITE"\n", inputFileNamep, outputFileNamep);
}
