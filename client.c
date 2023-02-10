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

#include "client.h"
#include "encryptor.h"
#include "tunnel.h"

#define MAX_PACKAGE_SIZE 4096

// tunnel (interceptor)
int tunnel_fd;
char tunnel_name[IFNAMSIZ];// name of the tunnel used in the interception
pthread_mutex_t lock;

/**
 * Client implementation
*/

// processing message callback function
void process_message(char* message, const void* arg);
void * receive_client();

int main(){
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
        return -1;
    }

    // CONFIGURING THE TUNNEL INTERCEPTOR
    memset(&package, 0, MAX_PACKAGE_SIZE);
    strcpy(tunnel_name, "tun0");
    tunnel_fd = tun_alloc(tunnel_name, IFF_TUN | IFF_NO_PI);
    if (tunnel_fd < 0){
        fprintf(stderr, "Tunnel setup error.\n");
        return -1;
    }

    pthread_t thread_receive;
    if(pthread_create(&thread_receive, NULL, &receive_client, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    while(1){
        message = "";
        message_pack = "";

        // intercepting package
        package_size = read(tunnel_fd, package, sizeof(package));
        pthread_mutex_lock(&lock);
        if(package_size < 0){
            fprintf(stderr, "Error capturing package.\n");
            return -1;
        }

        // printing the intercepted package (for debugging purposes)
        printf("\nPaquet à envoyer : ");
        for (int i = 0; i < package_size; i++)
            printf("%02hhX ", package[i]);
            // printf("%d ", package[i]);
	 printf("\n");

        // encrypting the package
        if (encrypt(package, package_size, &message_pack) < 0) {
            fprintf(stderr, "Encryption error.\n");
            return -1;
        }

        // constructing the telegram message
        message = (char *)malloc(
<<<<<<< HEAD
            strlen("client:") + 
            strlen(message_pack) +
            1
        );
        strcpy(message, "client:");
=======
            strlen("client:_") + 
            strlen(message_pack) +
            1
        );
        strcpy(message, "client:_");
>>>>>>> Ajout de la fonctionnalité de retour sur le client (manque la gestion de qui récupère quel message)
        strcat(message, message_pack);

        // sending the message in the chat
        if(send_message(message, &config) < 0){
            fprintf(stderr, "Error sending message.\n");
            return -1;
        }
        pthread_mutex_unlock(&lock);
    }


    // cleanup
    free(message);
    free(message_pack);
    free(config.bot_id);
    free(config.chat_id);

    return 1;
}

void * receive_client(){
    config_t config;
    int frequency = 1;

    config.frequency = &frequency;
    config.local_ip = NULL;
    if(setup(&config, "CLIENT_BOT_ID", "TELEGRAM_CHAT_ID") < 0){
        fprintf(stderr, "Server bot setup error.\n");
        return NULL;
    }

    if(read_posts(process_message, &tunnel_fd, &config) < 0){
        fprintf(stderr, "Could bot read posts.\n");
        return NULL;
    }

    free(config.bot_id);
    free(config.chat_id);

    return NULL;
}

void process_message(char* message, const void* arg){
    // package variables
    byte* package = malloc(1);
    int package_size;
    int nwrite = 0;

    printf("\nprocessing and receiving message:\n%s\n", message);

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

