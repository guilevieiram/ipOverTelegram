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
#include "client.h"
#include "encryptor.h"

#define MAX_PACKAGE_SIZE 4096

/**
 * Client implementation
*/

int tun_alloc(char *dev, int flags);

int main(){

    // tunnel (interceptor)
    int tunnel_fd;
    char tunnel_name[IFNAMSIZ];// name of the tunnel used in the interception

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

    while(1){
        message = "";
        message_pack = "";

        // intercepting package
        package_size = read(tunnel_fd, package, sizeof(package));
        if(package_size < 0){
            fprintf(stderr, "Error capturing package.\n");
            return -1;
        }

        // printing the intercepted package (for debugging purposes)
        for (int i = 0; i < package_size; i++)
            printf("%02hhX ", package[i]);
            // printf("%d ", package[i]);

        // encrypting the package
        if (encrypt(package, package_size, &message_pack) < 0) {
            fprintf(stderr, "Encryption error.\n");
            return -1;
        }

        // constructing the telegram message
        message = (char *)malloc(
            strlen("client: ") + 
            strlen(message_pack) +
            1
        );
        strcpy(message, "client: ");
        strcat(message, message_pack);

        // sending the message in the chat
        if(send_message(message, &config) < 0){
            fprintf(stderr, "Error sending message.\n");
            return -1;
        }
    }


    // cleanup
    free(message);
    free(message_pack);
    free(config.bot_id);
    free(config.chat_id);

    return 1;
}


int tun_alloc(char *dev, int flags){

    struct ifreq ifr;
    int fd, err;
    char *clonedev = "/dev/net/tun";

    /* Arguments taken by the function:
     *
     * char *dev: the name of an interface (or '\0'). MUST have enough
     *   space to hold the interface name if '\0' is passed
     * int flags: interface flags (eg, IFF_TUN etc.)
     */

    /* open the clone device */
    if ((fd = open(clonedev, O_RDWR)) < 0)
    {
        return fd;
    }

    /* preparation of the struct ifr, of type "struct ifreq" */
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = flags; /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

    if (*dev)
    {
        /* if a device name was specified, put it in the structure; otherwise,
         * the kernel will try to allocate the "next" device of the
         * specified type */
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    /* try to create the device */
    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0)
    {
        close(fd);
        return err;
    }

    /* if the operation was successful, write back the name of the
     * interface to the variable "dev", so the caller can know
     * it. Note that the caller MUST reserve space in *dev (see calling
     * code below) */
    strcpy(dev, ifr.ifr_name);

    /* this is the special file descriptor that the caller will use to talk
     * with the virtual interface */
    return fd;
}

