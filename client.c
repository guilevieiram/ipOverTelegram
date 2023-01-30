#include <stdio.h>
#include <stdlib.h>
#include "client.h"

#define BUFSIZE 160

/**
 * Client implementation
*/

int main(){
/**
 * SETTING UP CLIENT ENVS
 * 
 * Environment variables implementation from https://joequery.me/code/environment-variable-c/
*/
    config_t config;
    char *client_bot_id = "CLIENT_BOT_ID";
    char *chat_id = "TELEGRAM_CHAT_ID";

    int frequency = 1;

    config.bot_id = malloc(BUFSIZE);
    config.chat_id = malloc(BUFSIZE);
    config.frequency = &frequency;
    config.local_ip = NULL;


    // getting client bot identifier

    if(!getenv(client_bot_id)){
        fprintf(stderr, "The environment variable %s was not found.\n", client_bot_id);
        return -1;
    }
    if(snprintf(config.bot_id, BUFSIZE, "%s", getenv(client_bot_id)) >= BUFSIZE){
        fprintf(stderr, "BUFSIZE of %d was too small. Aborting\n", BUFSIZE);
        return -1;
    }

    if(!getenv(chat_id)){
        fprintf(stderr, "The environment variable %s was not found.\n", chat_id);
        return -1;
    }
    if(snprintf(config.chat_id, BUFSIZE, "%s", getenv(chat_id)) >= BUFSIZE){
        fprintf(stderr, "BUFSIZE of %d was too small. Aborting\n", BUFSIZE);
        return -1;
    }

    printf("bot_id: %s, chat_id: %s\n", config.bot_id, config.chat_id);

    send_message("Yoooo man message sent from C", &config);
    
    return 1;
}