#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

#include "server.h"
#include "encryptor.h"


/**
 * Server implementation
*/

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

void process_message(char* message, const void* arg){
    // package variables
    byte* package;
    int package_size;
    int nwrite = 0;

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
    nwrite = write(*(int *)arg, package, sizeof(package));
    if(nwrite < 0){
        fprintf(stderr, "Error capturing package.\n");
        return;
    }
    printf("bytes writen in the tunnel: %d", nwrite);

    // int sockfd;
    // struct sockaddr_in next_hop_router;

    // // Create a raw socket
    // sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    // if (sockfd < 0) {
    //     perror("socket");
    //     return;
    // }

    // // Fill in the sockaddr_in structure for the next hop router
    // memset(&next_hop_router, 0, sizeof(next_hop_router));
    // next_hop_router.sin_family = AF_INET;
    // next_hop_router.sin_addr.s_addr = inet_addr("0.0.0.0");


    // // Send the packet to the next hop router
    // if (sendto(sockfd, package, package_size, 0, (struct sockaddr*)&next_hop_router, sizeof(next_hop_router)) < 0) {
    //     perror("sendto");
    //     return;
    // }

    return;
}

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
        return;
    }

    if(read_posts(process_message, &tunnel_fd, &config) < 0){
        fprintf(stderr, "Could bot read posts.\n");
        return -1;
    }

    free(config.bot_id);
    free(config.chat_id);

    return 1;
}