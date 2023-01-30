#include <stdio.h>
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


int setup(const config_t* config){
    printf("Setup of bot of id: %s\n", config->bot_id);
    return 1;
}

int read_posts(void (*process_response)(char *, const void *), const void* args, const config_t* config){
    printf("Reading posts\n");

    char url[MAXREQ] = "";
    char* response,
        *date,
        *text;

    char recent_text[MAXTEXT];
    size_t timestamp, old_timestamp = 0;

    
    strcat(url, "https://api.telegram.org/bot");
    strcat(url, config->bot_id);
    strcat(url, "/getUpdates");
    
    while(1){ // rudimentary implementation but it does go over firewalls
        response = get(url);

        // processing result
        if(strstr(response, "\"ok\":true") == NULL){
            fprintf(stderr, "Message could not be read\n");
            return -1;
        }

        // while(parse_response(response, &text, &date))
        while(parse_response(&response, &text, &date) > 0){
            if(timestamp < atoi(date)) {
                timestamp = atoi(date);
                strcpy(recent_text, text);
            }
        }

        // checking only for new messages
        if (timestamp != old_timestamp){
            old_timestamp = timestamp;
            process_response(recent_text, args);
        }

        sleep(1/config->frequency);
    }

    
    return 1;
}

int send_message(const char* message, const config_t* config){
    char url[MAXREQ] = "";
    char* response;

    strcat(url, "https://api.telegram.org/bot");
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

    return 1;
}
