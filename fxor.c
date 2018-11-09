//Author: Ryan Houck
//Last modificaiton: Nov. 9, 2018

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(unix) || defined(__unix) || defined(__unix__)
#include <unistd.h>
#endif

#define ROTATE_RIGHT 2
#define ROTATE_LEFT 3

/**
 * Rotates val right by amnt
 * @param val The value to be rotated
 * @param amnt The amount to be rotated by
 * @return The rotated value
 */
char rotateRight(char val, int amnt) {
    char sign = !!(val >> 7); //Preserves the sign bit; 0x1 if sign bit is positive, 0x0 otherise
    return ((val & 0x7f) >> amnt) + (val << (8 - amnt)) + (sign << (7 - amnt));
}

/**
 * Rotates val left by amnt
 * @param val The value to be rotated
 * @param amnt The amount to be rotated by
 * @return The rotated value
 */
char rotateLeft(char val, int amnt) {
    char sign = !!(val >> 7);
    return ((val & 0x7f) << amnt) + (val >> (8 - amnt)) + (sign << (7 - amnt));
}

/**
 * xor's the key and the string, along with rotating and re-xoring the key over time
 * @param str The string that is being xor'd
 * @param strSize The size of the string being xor'd
 * @param key The key that is being used to xor the string
 * @param keylen The length of the key that is being used to xor the string
 * @return The xor'd version of the string
 */
char* xor(const char* str, int strSize, const char* key, int keylen){
    char* tempKey = malloc(keylen+1);
    for(int i = 0; i < keylen; i++){
        tempKey[i] = key[i];
    }
    char* tempStr = malloc(strSize+1);
    for(int i = 0; i < strSize; i++){
        tempStr[i] = str[i];
    }
    for(int i = 0; i < strSize; i++){
        tempKey[i%keylen] = (i%2 == 0)? rotateRight(tempKey[i%keylen], i%(ROTATE_RIGHT%8)) : rotateLeft(tempKey[i%keylen], i%(ROTATE_LEFT%8));
        tempStr[i] ^= tempKey[i%keylen];
        tempKey[i%keylen] ^= key[i%keylen];
    }
    free(tempKey);
    return tempStr;
}

//ARGUMENTS
//argv[1] - Option (-e/-d)
//argv[2] - The name of the file to be encrypted/decrypted
//argv[3] - The key file or the length of key (depending on encryption or decryption)
//
//OPTIONAL ARGUMENTS
//argv[4] - The output file
int main(int argc, char** argv){
    if((argc != 4 && argc != 5) || ((strcmp(argv[1], "-d") != 0 && strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-D") != 0 && strcmp(argv[1], "-E") != 0))){
        printf("Please use one of the folloing formats:\n\nDecryption:\n./fxor -d <filename_in> <key_file>\n\nOR\n\nEncryption:\n./fxor -e <filename_in> <key_length>\n\n");
        return 1;
    }
    //Reading in the string from the given file
    char* str = malloc(1);
    int fileNameLen = strlen(argv[2]);
    FILE* file = fopen(argv[2], "r");
    if(file == NULL){
        printf("Could not find the file \"%s\".\n", argv[2]);
        return 2;
    }
    char tempChar = getc(file);
    int strSize = 0;
    while(!feof(file)){
        str[strSize++] = tempChar;
        tempChar = getc(file);
        str = realloc(str, strSize+1);
    }
    fclose(file);

    char* key;
    int keylen = 0;
    //If decoding, read in the string from the given file
    if(strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-D") == 0){
        file = fopen(argv[3], "r");
        if(file == NULL){
            printf("Could not find the file \"%s\".\n", argv[3]);
            return 2;
        }
        tempChar = getc(file);
        key = malloc(1);
        while(!feof(file)){
            key[keylen++] = tempChar;
            tempChar = getc(file);
            key = realloc(key, keylen+1);
        }
        fclose(file);
    }
    //If encoding, generate the key and write the key out to a file
    else{
        //Seeding off of time and PID if the system is unix-based
        #if defined(unix) || defined(__unix) || defined(__unix__)
        srand(time(NULL)^getpid());
        #else
        srand(time(NULL));
        #endif
        char* keyFile = malloc(((argc == 5)? strlen(argv[4]) : fileNameLen) + 5);
        strcpy(keyFile, (argc == 5)? argv[4] : argv[2]);
        strcat(keyFile, ".key\0");
        key = malloc(atoi(argv[3]));
        file = fopen(keyFile, "w");
        if(file == NULL){
            printf("Could not open %s for writing.\n", keyFile);
        }
        for(; keylen < atoi(argv[3]); keylen++){
            key[keylen] = rand()%256;
            fprintf(file, "%c", key[keylen]);
        }
        fclose(file);
        printf("key successfully written to %s\n", keyFile);
    }
    
    //If the key is not 0 length (no encryption), run the xor
    if(strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-D") == 0 || atoi(argv[3]) != 0){
        str = xor(str, strSize, key, keylen);
    }

    //Generating the output file
    char* outFileStr;
    if(argc != 5){
        if(strcmp(&(argv[2][fileNameLen-4]), ".enc") == 0){
            outFileStr = malloc(fileNameLen-4);
            for(int i = 0; i < fileNameLen-4; i++){
                outFileStr[i] = argv[2][i];
            }
            outFileStr[fileNameLen-4] = '\0';
        }
        else{
            outFileStr = malloc(fileNameLen + 5);
            strcpy(outFileStr, argv[2]);
            strcat(outFileStr, ".enc\0");
        }
    }
    file = fopen((argc == 5)? argv[4] : outFileStr, "w");
    if(file == NULL){
        printf("Could not open the file for writing.\n");
        for(int i = 0; i < strSize; i++){
            printf("%c", str[i]);
        }
        printf("\n");
        return 2;
    }
    else{
        for(int i = 0; i < strSize; i++){
            fprintf(file, "%c", str[i]);
        }
        fclose(file);
        printf("Output successfully written to %s\n", (argc == 5)? argv[4] : outFileStr);
    }
    return 0;
}
