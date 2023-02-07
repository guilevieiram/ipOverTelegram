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
#include <pthread.h>

#include "server.h"
#include "encryptor.h"
#include "tunnel.h"

#define MAX_PACKAGE_SIZE 4096

int tunnel_fd;
char tunnel_name[IFNAMSIZ];
pthread_mutex_t lock;

/**
 * Server implementation
*/

// processing message callback function
void process_message(char* message, const void* arg);
void * listen_server();

int main(){
    // pthread_t thread_listen;
    // if(pthread_create(&thread_listen, NULL, &listen_server, NULL)) {
    //     fprintf(stderr, "Error creating thread\n");
    //     return 1;
    // }

    // pthread_join(thread_listen, NULL);

    config_t config;
    int frequency = 1;

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
    printf("%d\n",tunnel_fd);

    pthread_t thread_listen;
    if(pthread_create(&thread_listen, NULL, &listen_server, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    if(read_posts(process_message, &tunnel_fd, &config) < 0){
        fprintf(stderr, "Could bot read posts.\n");
        return -1;
    }

    free(config.bot_id);
    free(config.chat_id);

    return 1;
}


// Fonction qui permet au serveur d'écouter la réponse
void * listen_server(){


    // package
    byte package[MAX_PACKAGE_SIZE];
    int package_size;

    //telegram message
    char* message_pack = (char *)malloc(1);
    char* message = (char *)malloc(1);

    // bot configuration  
    config_t config;
    int frequency = 1;

    // CONFIGURING THE BOT
    config.frequency = &frequency;
    config.local_ip = NULL;
    if(setup(&config, "CLIENT_BOT_ID", "TELEGRAM_CHAT_ID") < 0){
        fprintf(stderr, "Client bot setup error.\n");
        return NULL;
    }

    while(1){
        message = "";
        message_pack = "";
        printf("arrivé ici1\n");
        // intercepting package
        printf("%d\n",tunnel_fd);
        package_size = read(tunnel_fd, package, sizeof(package));
        // on vérouille pour ne pas mélanger réception et envoi
        pthread_mutex_lock(&lock);
        if(package_size < 0){
            fprintf(stderr, "Error capturing package.\n");
            return NULL;
        }
        

        // printing the intercepted package (for debugging purposes)
        for (int i = 0; i < package_size; i++)
            printf("%02hhX ", package[i]);
            // printf("%d ", package[i]);

        // encrypting the package
        if (encrypt(package, package_size, &message_pack) < 0) {
            fprintf(stderr, "Encryption error.\n");
            return NULL;
        }

        // constructing the telegram message
        message = (char *)malloc(
            strlen("server:") + 
            strlen(message_pack) +
            1
        );
        strcpy(message, "server:");
        strcat(message, message_pack);

        // sending the message in the chat
        if(send_message(message, &config) < 0){
            fprintf(stderr, "Error sending message.\n");
            return NULL;
        }
        pthread_mutex_unlock(&lock);
    }
    printf("%s",message);


    // cleanup
    free(message);
    free(message_pack);
    free(config.bot_id);
    free(config.chat_id);

    return NULL;
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
    pthread_mutex_lock(&lock);
    nwrite = write(*(int *)arg, (char *)&plen, sizeof(plen));
    nwrite = write(*(int *)arg, package, package_size);
    pthread_mutex_unlock(&lock);
    if(nwrite < 0){
        fprintf(stderr, "Error injecting package.\n");
        return;
    }
    printf("Bytes written in the tunnel: %d\n", nwrite);

    return;
}
