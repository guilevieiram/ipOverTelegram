#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encryptor.h"

int encrypt(const byte* bytes, char** destination){
    // lenght of the input byte array
    int length = strlen((char *)bytes);

    // temporary pointer for the destination
    char* temporary_pointer = *destination;

    // temporary byte variable for storing
    byte temporary_byte;

    // for all the bytes
    for(int i = 0; i < length; i++){

        // reading the byte
        temporary_byte = bytes[i];

        // allocating the necessary memory
        // the +1 to account for spaces
        temporary_pointer = malloc(
            strlen(*destination) + 
            strlen(_dictionary[(int)temporary_byte]) + 
            1
        );

        // recopying the information in the newly allocated memory
        strcpy(temporary_pointer, *destination);
        strcat(temporary_pointer, _dictionary[(int)temporary_byte]);
        strcat(temporary_pointer, " ");

        // remaking the pointer
        *destination = temporary_pointer;
    }
    return 1;
}

int decrypt(char* input, byte** output){
    // loop variable to count the current byte
    int byte_index = 0;

    // string delimiter
    const char delimiter[] = " ";

    // storing the words in the string
    char* word = strtok(input, delimiter);

    // temporary bytes pointer
    byte* bytes_pointer = *output;

    while(word != NULL){
        
        // finding the index of the desired word
        for(byte_index = 0; byte_index < VOCABULARY_LEN; byte_index++){
            if(strcmp(word, _dictionary[(int)byte_index]) == 0) 
                break;
        }
        
        // error catching
        if(byte_index == VOCABULARY_LEN){
            printf("ERROR: word not in dictionary.\n");
            return -1;
        }

        // allocating the nedded memory (1 more than before)
        bytes_pointer = malloc(strlen(*output) + 1);

        // copying the previous bytes
        strcpy(bytes_pointer, *output);

        // inserting the new byte at the end
        bytes_pointer[strlen(*output)] = (char)byte_index;

        // updating the output
        *output = bytes_pointer;

        // getting the next word
		word = strtok(NULL, delimiter);
    }
    return 1;
}
