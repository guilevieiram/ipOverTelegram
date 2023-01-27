#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encryptor.h"

int encrypt(const byte* bytes, char** destination){
    int len = strlen((char *)bytes);
    char* ptr = *destination;
    for(int i = 0; i < len; i++){
        ptr = malloc(strlen(*destination) + strlen(_dictionary[(int)bytes[i]]) + 1);
        strcpy(ptr, *destination);
        strcat(ptr, _dictionary[(int)bytes[i]]);
        strcat(ptr, " ");
        *destination = ptr;
    }
    return 1;
}

int decrypt(char* input, byte** output){
    int i;
    char delimiter[] = " ";
    char* ptr = strtok(input, delimiter);
    byte* byte_ptr = *output;
    while(ptr != NULL){
        for(i = 0; i < VOCABULARY_LEN; i++)
            if(strcmp(ptr, _dictionary[(int)i]) == 0) break;
        byte_ptr = malloc(strlen(*output) + 1);
        strcpy(byte_ptr, *output);
        byte_ptr[strlen(*output)] = (char)i;
        *output = byte_ptr;
		ptr = strtok(NULL, delimiter);
    }
    return 1;
}
