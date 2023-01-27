#include <stdio.h>
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
    char * res = get("https://google.com");
    printf("%s\n", res)

    // byte bytes[5] = "asdf"; 
    // char* string_out = "";
    // encrypt((byte *)bytes, &string_out);
    // for(int i = 0 ; i < (int)strlen(string_out); i++)
    //     printf("out enc: %c\n", string_out[i]);



    // char string[30] = "small large next";
    // byte* bytes_out = "";
    // decrypt(string, &bytes_out);
    // for(int i = 0 ; i < (int)strlen(bytes_out); i++)
    //     printf("out dec: %c\n", bytes_out[i]);
    // printf("out dec: %s \n", bb);

    return 1;
}