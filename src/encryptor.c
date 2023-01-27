#include <stdio.h>
#include <string.h>
#include "encryptor.h"

int encrypt(const byte* bytes, char** destination){
    char* len = strlen((char *)bytes);
    char* ptr = *destination;
    for(int i = 0; i < len; i++){
        ptr = malloc(strlen(*destination) + strlen(_dictionary[bytes[i]]) + 1);
        strcpy(ptr, *destination);
        strcat(ptr, _dictionary[bytes[i]]);
        strcat(ptr, " ");
        *destination = ptr;
    }
    return 1;
}

int decrypt(const char* input, byte** output){
    return 1;
}
