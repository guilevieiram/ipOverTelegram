#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/if_tun.h>
#include <net/if.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "server.h"
#include "encryptor.h"
#include "tunnel.h"


/**
 * Server implementation
*/

// processing message callback function
void process_message(char* message, const void* arg);

int main(){
    config_t config;
    int frequency = 1;
    int tunnel_fd;
    char tunnel_name[IFNAMSIZ];// name of the tunnel used in the interception

    config.frequency = &frequency;
    config.local_ip = NULL;
    if(setup(&config, "SERVER_BOT_ID", "TELEGRAM_CHAT_ID") < 0){
        fprintf(stderr, "Server bot setup error.\n");
        return -1;
    }

    strcpy(tunnel_name, "tun1");
    tunnel_fd = tun_alloc(tunnel_name, IFF_TUN | IFF_NO_PI);
    if (tunnel_fd < 0){
        fprintf(stderr, "Tunnel setup error.\n");
        return -1;
    }

    if(read_posts(process_message, &tunnel_fd, &config) < 0){
        fprintf(stderr, "Could bot read posts.\n");
        return -1;
    }

    free(config.bot_id);
    free(config.chat_id);

    return 1;
}


void process_message(char* message, const void* arg){
    // package variables
    byte* package = malloc(1);
    int package_size;
    int nwrite = 0;

    printf("\nprocessing:\n%s\n", message);

    // error checking and updating message
    if(message == NULL) return;
    if(strstr(message, "client:") == NULL) return;
    message += strlen("client:");

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
    int plen = htons(package_size);
    nwrite = write(*(int *)arg, (char *)&plen, sizeof(plen) );
    nwrite = write(*(int *)arg, package, package_size);
    if(nwrite < 0){
        fprintf(stderr, "Error injecting package.\n");
        return;
    }
    printf("Bytes written in the tunnel: %d\n", nwrite);

    return;
}
