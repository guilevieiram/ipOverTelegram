#include <stdio.h>
#include "telegram.h"


int setup(const config_t* config){
    printf("Setup of bot\n");
    return 1;
}

int read_posts(const void process_response(char *, const void *), const void* args, const config_t* config){
    printf("Reading posts\n");
    return 1;
}

int send_message(const char* message, const config_t* config){
    printf("Sending message\n");
    return 1;
}

int main(){
    printf("in telegram");
    return 1;
}