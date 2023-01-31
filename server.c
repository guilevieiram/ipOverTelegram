#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "encryptor.h"


/**
 * Server implementation
*/

void process_message(char* message, const void* arg){
    // package variables
    byte* package;
    int package_size;

    printf("\nprocessing:\n%s\n", message);

    // error checking and updating message
    if(message == NULL) return;
    if(strstr(message, "client: ") == NULL) return;
    message += strlen("client: ");

    // decrypting message
    if((package_size = decrypt(message, strlen(message), &package)) < 0){
        fprintf(stderr, "Server could not decrypt received message.\n");
        return;
    }


    // printing the package data for debugging
    // this part should reinsert the package back in the network
    printf("dump: \n");
    for (int i = 0; i < package_size; i++)
        printf("%02hhX ", package[i]);
    printf("\nenddump \n");

    return;
}

int main(){
    config_t config;
    int frequency = 1;

    config.frequency = &frequency;
    config.local_ip = NULL;
    if(setup(&config, "SERVER_BOT_ID", "TELEGRAM_CHAT_ID") < 0){
        fprintf(stderr, "Server bot setup error.\n");
        return -1;
    }

    if(read_posts(process_message, NULL, &config) < 0){
        fprintf(stderr, "Could bot read posts.\n");
        return -1;
    }

    free(config.bot_id);
    free(config.chat_id);

    return 1;
}