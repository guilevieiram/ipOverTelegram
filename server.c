#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

    // TESTING TO REINSERT PACKET

    int sockfd;
    struct sockaddr_in next_hop_router;

    // Create a raw socket
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    // Fill in the sockaddr_in structure for the next hop router
    memset(&next_hop_router, 0, sizeof(next_hop_router));
    next_hop_router.sin_family = AF_INET;
    next_hop_router.sin_addr.s_addr = inet_addr("192.168.1.1");


    // Send the packet to the next hop router
    if (sendto(sockfd, package, package_size, 0, (struct sockaddr*)&next_hop_router, sizeof(next_hop_router)) < 0) {
        perror("sendto");
        return;
    }

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