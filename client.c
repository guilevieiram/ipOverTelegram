#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "encryptor.h"


/**
 * Client implementation
*/

int main(){
    // to be change for the IP package
    byte package[] = "cole tuzin, tudo bao?";

    char* message_pack = "";
    char* message = (char *)malloc(1);

    config_t config;
    int frequency = 1;

    config.frequency = &frequency;
    config.local_ip = NULL;

    if(setup(&config, "CLIENT_BOT_ID", "TELEGRAM_CHAT_ID") < 0){
        fprintf(stderr, "Client bot setup error.\n");
        return -1;
    }

    if (encrypt(package, &message_pack) < 0) {
        fprintf(stderr, "Encryption error.\n");
        return -1;
    }

    message = (char*)malloc(
        strlen("client: ") + 
        strlen(message_pack) +
        1
    );
    strcpy(message, "client: ");
    strcat(message, message_pack);

    if(send_message(message, &config) < 0){
        fprintf(stderr, "Error sending message.\n");
        return -1;
    }

    free(message);
    free(message_pack);
    free(config.bot_id);
    free(config.chat_id);

    return 1;
}