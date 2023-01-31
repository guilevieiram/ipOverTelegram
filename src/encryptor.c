#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encryptor.h"

int encrypt(const byte* bytes, const int length, char** destination){

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
            strlen(_dictionary[(int)temporary_byte + 128]) + 
            strlen(" ") + 
            1
        );

        // recopying the information in the newly allocated memory
        strcpy(temporary_pointer, *destination);
        strcat(temporary_pointer, _dictionary[(int)temporary_byte + 128]);
        strcat(temporary_pointer, " ");

        // remaking the pointer
        *destination = temporary_pointer;
    }

    return 1;
}

int decrypt(const char* input, const int length, byte** output){
    // count the output size
    int number_bytes = 0;

    // loop variable to count the current byte
    int byte_index = 0;

    // temporary bytes pointer
    byte* bytes_pointer = *output;

    // string delimiter
    const char delimiter[] = " ";

    // copied string
    char* string = (char *)malloc(length + 1);
    strcpy(string, input);

    // storing the words in the string
    char* word = strtok(string, delimiter);

    while(word != NULL){
        // finding the index of the desired word
        for(byte_index = 0; byte_index < VOCABULARY_LEN; byte_index++){
            if(strcmp(word, _dictionary[(int)byte_index]) == 0) 
                break;
        }

        // updating number of bytes read
        number_bytes ++;
        
        // error catching
        if(byte_index == VOCABULARY_LEN){
            printf("ERROR: word {%s} not in dictionary.\n", word);
            return -1;
        }

        // allocating the needed memory (1 more than before)
        bytes_pointer = (char *)malloc(number_bytes + 1);

        // copying the previous bytes
        for(int i = 0; i < number_bytes; i++)
            bytes_pointer[i] = (*output)[i];

        // inserting the last byte
        bytes_pointer[number_bytes] = (char) (byte_index - 128);

        // updating the output
        *output = bytes_pointer;

        // getting the next word
		word = strtok(NULL, delimiter);
    }

    // freeing the first byte
    (*output) ++;

    free(string);

    return number_bytes;
}
