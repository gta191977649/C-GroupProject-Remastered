#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileCompressor.h"
#include "Encryption.h"
#include "Common.h"

#define MAX_FILENAME_LEN 256
#define COMPRESS_OUTPUT_FILENAME "compressed.txt"
#define DECOMPRESS_OUTPUT_FILENAME "out.txt"
void printMenu(void);
void compressFile();
void decompressFile();
void toogleDebugMode();
void encryptFile();
void decryptFile();
void exitProgram();
/******************************************************************************
 main
 This function is main function and read user input
 inputs:
  - none
 output:
  - if user input is 6, return 0 and end
 *****************************************************************************/
int main(void)
{
    /*Test*/
	int choice = 0;
	while(choice != 6)
   	{
        switch(choice)
        {
            case 1: 
                /* Encrypt */
                encryptFile();
                break;
            case 2: 
            	/* Decrypt */
                decryptFile();
                break;
            case 3:
                /* Compress */
                compressFile();
                break;
            case 4: 
                /* Encrypt and Compress */
                decompressFile();
                break;
            case 5:
                /* Debug mode */
                toogleDebugMode();
                break;
            default:
                printf("Invalid choice\n");
                break;
        }	
        printMenu();
        scanf("%d", &choice);
    }
    exitProgram();
    return 0;
}
/******************************************************************************
 printMenu
 This function print the user menu in console
 inputs:
  - none
 output:
  - none
 *****************************************************************************/
void printMenu(void)
{
	printf("\n"
	"     __-------------Title-------------__\n"
	"     |		          By: Group 27 |\n"	
    "     |1. Encrypt the file              |\n"
    "     |2. Decrypt the file              |\n"
    "     |3. Compress the file             |\n"
    "     |4. Decompress the file           |\n"
    "     |5. Toggle debug mode             |\n"
    "     |6. Exit the program              |\n"
    "     |---------------------------------|\n"
    "Enter choice>");
}
/******************************************************************************
 toogleDebugMode
 This function use for toggle debug mode
 inputs:
  - none
 output:
  - none
 *****************************************************************************/
void toogleDebugMode(){
    char options;
    do {
        printf(debugMode ? "Current Debug mode: "
            GREEN"[Enabled]"WHITE"\n" : "Current Debug mode: "
            RED"[Disabled]"WHITE"\n" );
        printf("Enter y for set enable, n for set disable.");
        printf("Your chouse > ");
        scanf("%c",&options);
    } while (options != 'y' && options != 'n');
    debugMode = options == 'y' ? 1 : 0;
    printf(debugMode ? "Debug mode set to: "GREEN"[Enabled]"
    WHITE"\n" : "Debug mode set to: "RED"[Disabled]"WHITE"\n" );
}
/******************************************************************************
 exitProgram
 This function use for exit program
 inputs:
  - none
 output:
  - none
 *****************************************************************************/
void exitProgram() {
    printf("Goodbye, have a gooday!\n");
}
/******************************************************************************
 compressFile
 This function use for input the filepath user want to compress
 inputs:
  - none
 output:
  - none
 *****************************************************************************/
void compressFile() {
    /* first, get the filename that the user wants to compress */
    printf("Please enter the file name you wish to compress: \n");
    char fileName[MAX_FILENAME_LEN];
    scanf("%s",fileName);

    fileinfo_t *fileinfo;
    fileinfo = mallocFileInfo();
    /*
    char *fileNameBuffer = (char*) malloc((strlen(fileName) + 
        strlen(COMPRESS_OUTPUT_PREFIX)) * sizeof(char*));
    strcat(fileNameBuffer,fileName);
    strcat(fileNameBuffer,COMPRESS_OUTPUT_PREFIX);
    printf("%s",fileNameBuffer);
    */
    
    compressorProcessFile(fileinfo, fileName, COMPRESS_OUTPUT_FILENAME);
    freeFileInfo(fileinfo);
    /*free(fileNameBuffer);*/
    /*printf("%s",fileName);*/
}
/******************************************************************************
 decompressFile
 This function use for input the filepath user want to decompress
 inputs:
  - none
 output:
  - none
 *****************************************************************************/
void decompressFile() {
    printf("Please enter the file name you wish to decompress: \n");
    char fileName[MAX_FILENAME_LEN];
    scanf("%s",fileName);
    fileinfo_t *fileInfo;
    fileInfo = mallocFileInfo();
    decompressorProcessFile(fileInfo, fileName,DECOMPRESS_OUTPUT_FILENAME);
    freeFileInfo(fileInfo);
}
/******************************************************************************
 decompressFile
 This function use for input the filepath user want to encrypt
 inputs:
  - none
 output:
  - none
 *****************************************************************************/
void encryptFile() {
    printf("Please enter the file name you wish to encrypt: \n");
    char fileName[MAX_FILENAME_LEN];
    scanf("%s",fileName);
    int password;
    printf("Please enter the password for the file (only number): \n");
    scanf("%d",&password);
    encryptWriteToFile(fileName,password);
}
/******************************************************************************
 decompressFile
 This function use for input the filepath user want to decrypt
 inputs:
  - none
 output:
  - none
 *****************************************************************************/
void decryptFile() {
    printf("Please enter the file name you wish to decrypt: \n");
    char fileName[MAX_FILENAME_LEN];
    scanf("%s",fileName);
    int password;
    printf("Please enter the password for the file (only number): \n");
    scanf("%d",&password);
    decriptWriteToFile(fileName,password);
}
