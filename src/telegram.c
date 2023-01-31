#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "telegram.h"
#include "request.h"


int parse_response(char** response, char** text, char** date){
    char *date_end, *text_end;
    *date = strstr(*response, "\"date\":");
    if(*date == NULL) return -1;
    *date += strlen("\"date\":");
    date_end = strstr(*date, ",");
    *date_end = '\0';


    *text = strstr(date_end + 1, "\"text\":\"");
    if(*text == NULL) return -1;
    *text += strlen("\"text\":\"");
    text_end = strstr(*text, "\"");
    *text_end = '\0';

    *response = text_end + 1;

    return 1;
}


int setup(config_t* config, char* bot_id_key, char* chat_id_key){
    /**
     * SETTING UP CLIENT ENVS
     * 
     * Environment variables implementation from https://joequery.me/code/environment-variable-c/
    */
    config->bot_id = malloc(ENVBUFF);
    config->chat_id = malloc(ENVBUFF);
    if(!getenv(bot_id_key)){
        fprintf(stderr, "The environment variable %s was not found.\n", bot_id_key);
        return -1;
    }
    if(snprintf(config->bot_id, ENVBUFF, "%s", getenv(bot_id_key)) >= ENVBUFF){
        fprintf(stderr, "ENVBUFF of %d was too small. Aborting\n", ENVBUFF);
        return -1;
    }
    if(!getenv(chat_id_key)){
        fprintf(stderr, "The environment variable %s was not found.\n", chat_id_key);
        return -1;
    }
    if(snprintf(config->chat_id, ENVBUFF, "%s", getenv(chat_id_key)) >= ENVBUFF){
        fprintf(stderr, "ENVBUFF of %d was too small. Aborting\n", ENVBUFF);
        return -1;
    }
    return 1;
}

int read_posts(void (*process_response)(char *, const void *), const void* args, const config_t* config){
    printf("Reading posts\n");

    char* url = (char*)malloc(
        strlen("https://api.telegram.org/bot") + 
        strlen(config->bot_id) + 
        strlen("/getUpdates") + 1
    );

    char* response = "",
        *date = "",
        *text = "";

    char* recent_text = malloc(1);
    size_t timestamp = 0, old_timestamp = 0;

    strcpy(url, "https://api.telegram.org/bot");
    strcat(url, config->bot_id);
    strcat(url, "/getUpdates");
    
    while(1){ // rudimentary implementation but it does go over firewalls
        response = get(url);

        if(response == NULL){
            fprintf(stderr, "Error in get.\n");
            return -1;
        }

        // processing result
        if(strstr(response, "\"ok\":true") == NULL){
            fprintf(stderr, "Message could not be read\n");
            return -1;
        }

        // parsing response, saving the latest
        while(parse_response(&response, &text, &date) > 0){
            if(timestamp < (size_t)atoi(date)) {
                timestamp = atoi(date);
                recent_text = malloc(strlen(text) + 2);
                strcpy(recent_text, text);
            }
        }

        // checking only for new messages
        if (timestamp != old_timestamp){
            old_timestamp = timestamp;
            process_response(recent_text, args);
        }

        sleep(1 / *config->frequency);
    }
    free(response);
    free(url);
    return 1;
}

int send_message(const char* message, const config_t* config){
    char* url = (char*)malloc(
        strlen("https://api.telegram.org/bot") + 
        strlen(config->bot_id) + 
        strlen("/sendMessage?chat_id=") + 
        strlen(config->chat_id) + 
        strlen("&text=") + 
        strlen(message) +
        1
    );
    char* response;

    strcpy(url, "https://api.telegram.org/bot");
    strcat(url, config->bot_id);
    strcat(url, "/sendMessage?chat_id=");
    strcat(url, config->chat_id);
    strcat(url, "&text=");
    strcat(url, message);
    
    response = get(url);
    printf("Response: %s\n", response);

    //processing the result
    if(strstr(response, "\"ok\":true") == NULL){
        fprintf(stderr, "Message could not be sent\n");
        return -1;
    }

    free(response);
    free(url);
    return 1;
}
