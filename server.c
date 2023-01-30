#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "encryptor.h"


/**
 * Server implementation
*/

void process_message(char* message, const void* arg){
    printf("processing: %s \n", message);
    byte* dump = "";

    if(message == NULL) return;
    if(strstr(message, "client: ") == NULL) return;
    message += strlen("client: ") ;

    if(decrypt(message, &dump) < 0){
        fprintf(stderr, "Server could not decrypt received message.\n");
    }

    printf("dump: %s\n", dump);
}

int main(){
    // 
    // SETTING UP SERVER ENVS
    // 
    // Environment variables implementation from https://joequery.me/code/environment-variable-c/
    //
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