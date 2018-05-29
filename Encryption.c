#include <stdio.h>
/* fclose(), fopen(), printf(), fgetc(), fputc(), putchar()*/
#include <stdlib.h>
/* free(), malloc() */
#include <string.h>
/* memcpy(), strcat(), strlen() */

#include "Encryption.h"
#include "Common.h"
/******************************************************************************
 encryptWriteToFile
 This function is write file need to encrypt into memery
 inputs:
  - filename: pointer to a char means the filepath 
  - password: pointer to a char which is password 
 output:
  - none
 *****************************************************************************/
void encryptWriteToFile(char *filename, int password)
{
    /*Add postfix to the filename */
    char * fileOutputname = (char *) malloc((strlen(filename) + 
        strlen(ENCRYPT_FILE_POSTFIX)) * sizeof(char *));
    memcpy(fileOutputname,filename,strlen(filename)+1);
    strcat(fileOutputname,ENCRYPT_FILE_POSTFIX);
    
    
    char result;
    FILE *fileReadp,*fileWritep;
    fileReadp = fopen(filename, "r");
    fileWritep = fopen(fileOutputname, "w");
    
    if(!fileReadp || !fileWritep) {
        printf("Error cannot open file: \n");
        return;
    }

    if(debugMode) printf("[Debug]: Content while encryption: \n");
    while ((result = fgetc(fileReadp)) != EOF)
    {
        result ^= password;
        if(debugMode) putchar(result);
        fputc(result, fileWritep);
    }
    fclose(fileReadp);
    fclose(fileWritep);
    
    printf("Encrption Finish, encrypted file is stored as "
        GREEN"%s"WHITE"\n",fileOutputname);
    free(fileOutputname);
}
/******************************************************************************
 decriptWriteToFile
 This function is write file need to decrypt into memery
 inputs:
  - filename: pointer to a char means the filepath 
  - password: pointer to a char which is password 
 output:
  - none
 *****************************************************************************/
void decriptWriteToFile(char *filename, int password)
{
    /*Add postfix to the filename */
    char * fileOutputname = (char *) malloc((strlen(filename) + 
        strlen(ENCRYPT_FILE_POSTFIX)) * sizeof(char *));
    memcpy(fileOutputname,filename,strlen(filename)+1);
    strcat(fileOutputname,DECNCRYPT_FILE_POSTFIX);
   
    
    char result;
    FILE *fileReadp,*fileWritep;
    fileReadp = fopen(filename, "r");
    fileWritep = fopen(fileOutputname, "w");
    if(!fileReadp || !fileWritep) {
        printf("Error cannot open file: \n");
        return;
    }
    if(debugMode) printf("[Debug]: Content while decription: \n");
    while ((result = fgetc(fileReadp)) != EOF)
    {
        result ^= password;
        if(debugMode) putchar(result);
        fputc(result, fileWritep);
    }
    fclose(fileReadp);
    fclose(fileWritep);
    
    
    printf("decryption Finish, decrypted file is stored as "
        GREEN"%s"WHITE"\n",fileOutputname);
    free(fileOutputname);
}
