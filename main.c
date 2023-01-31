#include "encryptor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in next_hop_router;
    unsigned char packet[] = {
        0x45, 0x00, 0x00, 0x3C, 0x79, 0x0A, 0x40, 0x00, 0x40, 0x06, 0x14, 0x5F, 0x12, 0x82, 0x44, 0x00, 0x12, 0x82, 0x44, 0x4F, 0x95, 0xF4, 0x1F, 0x90, 0x9F, 0x77, 0xF9, 0x9B, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x02, 0xFA, 0xF0, 0x86, 0x59, 0x00, 0x00, 0x02, 0x04, 0x05, 0xB4, 0x04, 0x02, 0x08, 0x0A, 0xBB, 0x40, 0x0F, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07
    };
    int packet_len = sizeof(packet);

    // Create a raw socket
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // Fill in the sockaddr_in structure for the next hop router
    memset(&next_hop_router, 0, sizeof(next_hop_router));
    next_hop_router.sin_family = AF_INET;
    next_hop_router.sin_addr.s_addr = inet_addr("192.168.1.1");


    // Send the packet to the next hop router
    if (sendto(sockfd, packet, packet_len, 0, (struct sockaddr*)&next_hop_router, sizeof(next_hop_router)) < 0) {
        perror("sendto");
        return 1;
    }

    return 0;
}

/**
 * Main file
 *
 * Used for building and testing modules.
 *
 */
// int main(int argc, char **argv)
// {
//     int sockfd, netfd, optval=1;
//     struct sockaddr_in ;
//     // struct sockaddr_in remote, local;
//     // socklen_t remotelen = sizeof(remote);
//     unsigned char packet[] = {
//         0x45, 0x00, 0x00, 0x3C, 0x79, 0x0A, 0x40, 0x00, 0x40, 0x06, 0x14, 0x5F, 0x12, 0x82, 0x44, 0x00, 0x12, 0x82, 0x44, 0x4F, 0x95, 0xF4, 0x1F, 0x90, 0x9F, 0x77, 0xF9, 0x9B, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x02, 0xFA, 0xF0, 0x86, 0x59, 0x00, 0x00, 0x02, 0x04, 0x05, 0xB4, 0x04, 0x02, 0x08, 0x0A, 0xBB, 0x40, 0x0F, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07
//     };
//     int packet_len = sizeof(packet);

//     // Create a raw socket
//     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         perror("socket error");
//         return 1;
//     }

//     // if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0) {
//     //   perror("setsockopt()");
//     //   exit(1);
//     // }
    
//     // memset(&local, 0, sizeof(local));
//     // local.sin_family = AF_INET;
//     // local.sin_addr.s_addr = htonl(INADDR_ANY);
//     // local.sin_port = htons(55555);
//     // if (bind(sockfd, (struct sockaddr*) &local, sizeof(local)) < 0) {
//     //   perror("bind()");
//     //   exit(1);
//     // }
    
//     // if (listen(sockfd, 5) < 0) {
//     //   perror("listen()");
//     //   exit(1);
//     // }

//     // memset(&remote, 0, remotelen);
//     // if ((netfd = accept(sockfd, (struct sockaddr*)&remote, &remotelen)) < 0) {
//     //   perror("accept()");
//     //   exit(1);
//     // }


//     // Set up the destination address
//     // memset(&dest_addr, 0, sizeof(dest_addr));
//     // dest_addr.sin_family = AF_INET;
//     // dest_addr.sin_addr.s_addr = INADDR_BROADCAST;

//     // Send the packet
//     if (sendto(sockfd, packet, packet_len, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr)) < 0) {
//         perror("sendto error");
//         return 1;
//     }
//     // write(netfd, packet, packet_len);

//     // Close the socket
//     close(sockfd);
//     return 0;
// }
