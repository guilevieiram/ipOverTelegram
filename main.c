#include <stdio.h>
#include "encryptor.h"

/**
 * Main file
 * 
 * Used for building and testing modules.
 * 
*/
int main(){
    printf("main\n");

    byte b[4] = "0123";
    char* string = "";
    encrypt(&b[0], &string);
    printf("out : %s\n", string);
    byte* bb = "";
    decrypt(string, &bb);
    printf("out dec: %s \n", bb);

    return 1;
}