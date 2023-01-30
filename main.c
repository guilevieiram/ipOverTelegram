#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encryptor.h"
#include "request.h"

/**
 * Main file
 * 
 * Used for building and testing modules.
 * 
*/
int main(){
    printf("main\n");

    

    // char* url = "https://catfact.ninja/fact/";
    // char* res = get(url);
    // printf("%s\n", res);

    // byte bytes[5] = "asdfd"; 
    // char* string_out = "";
    // encrypt((byte *)bytes, &string_out);
    // printf("out: %s\n", string_out);
    // for(int i = 0 ; i < (int)strlen(string_out); i++)
    //     printf("out enc: %c\n", string_out[i]);



    char* string = "me when can make like time no just";
    byte* bytes_out = (byte *)malloc(1);
    decrypt(string, &bytes_out);
    for(int i = 0 ; i < (int)strlen(bytes_out); i++)
        printf("out dec: %c\n", bytes_out[i]);

    return 1;
}