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

#define BUFFER 3000

int tun_alloc(char *dev, int flags)
{

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

// int cread(int fd, char *buf, int n)
// {

//     int nread;

//     if ((nread = read(fd, buf, n)) < 0)
//     {
//         perror("Reading data");
//         exit(1);
//     }
//     return nread;
// }

/**************************************************************************
 * cwrite: write routine that checks for errors and exits if an error is  *
 *         returned.                                                      *
 **************************************************************************/

/**
 * Main file
 *
 * Used for building and testing modules.
 *
 */
int main(int argc, char **argv)
{
    char message[] = "self just just try well sun point just point help his our mean here need form mean here need same other any end as her children could them just just just just what sentence give little about grow just just sentence think say their think sentence line turn story do father little just just just just form great great low self just just try well four point just point help his give mean here need form mean here need same other any end as her children could them just just just just what sentence give little time point just just sentence think say their think sentence line turn story do should form just just just just form great great low self just just try well between point just point help his me mean here need form mean here need same other any end as her children could them just just just just what sentence give little how put just just sentence think say their think sentence line turn story do learn home just just just just form great great low self just just try well";
    byte* dump = "";

    decrypt(message, strlen(message), &dump);
    printf("%s\n", dump);
    
    // int sockfd;
    // struct sockaddr_in dest_addr;
    // unsigned char packet[] = {
    //     0x45, 0x00, 0x00, 0x3C, 0x79, 0x0A, 0x40, 0x00, 0x40, 0x06, 0x14, 0x5F, 0x12, 0x82, 0x44, 0x00, 0x12, 0x82, 0x44, 0x4F, 0x95, 0xF4, 0x1F, 0x90, 0x9F, 0x77, 0xF9, 0x9B, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x02, 0xFA, 0xF0, 0x86, 0x59, 0x00, 0x00, 0x02, 0x04, 0x05, 0xB4, 0x04, 0x02, 0x08, 0x0A, 0xBB, 0x40, 0x0F, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07
    // };
    // int packet_len = sizeof(packet);

    // // Create a raw socket
    // if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
    //     perror("socket error");
    //     return 1;
    // }

    // // Set up the destination address
    // memset(&dest_addr, 0, sizeof(dest_addr));
    // dest_addr.sin_family = AF_INET;
    // dest_addr.sin_addr.s_addr = INADDR_BROADCAST;

    // // Send the packet
    // if (sendto(sockfd, packet, packet_len, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr)) < 0) {
    //     perror("sendto error");
    //     return 1;
    // }

    // // Close the socket
    // close(sockfd);
    // return 0;


    // char pack[60] = {
    //     69,
    //     0,
    //     0,
    //     60,
    //     -70,
    //     126,
    //     64,
    //     0,
    //     64,
    //     6,
    //     -46,
    //     -22,
    //     18,
    //     -126,
    //     68,
    //     0,
    //     18,
    //     -126,
    //     68,
    //     79,
    //     -107,
    //     -10,
    //     31,
    //     -112,
    //     -18,
    //     -4,
    //     -24,
    //     -50,
    //     0,
    //     0,
    //     0,
    //     0,
    //     -96,
    //     2,
    //     -6,
    //     -16,
    //     -11,
    //     77,
    //     0,
    //     0,
    //     2,
    //     4,
    //     5,
    //     -76,
    //     4,
    //     2,
    //     8,
    //     10,
    //     -69,
    //     65,
    //     97,
    //     -38,
    //     0,
    //     0,
    //     0,
    //     0,
    //     1,
    //     3,
    //     3,
    //     7
    // };

    // int tun_fd,
    //      sock_fd, nread, nwrite, plength;
    // // char tun_name[IFNAMSIZ];
    // // char buffer[BUFFER];
    // struct sockaddr_in local;

    // // memset(&buffer, 0, BUFFER);

    // for (int i = 0; i < 60; i++)
    // {
    //     printf("%d ", pack[i]);
    // }

    // /* Connect to the device */
    // // strcpy(tun_name, "tun0");
    // // tun_fd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI); /* tun interface */

    // // if (tun_fd < 0)
    // // {
    // //     perror("Allocating interface");
    // //     exit(1);
    // // }

    // if ((sock_fd = socket(PF_INET, SOCK_RAW, 0)) < 0)
    // {
    //     fprintf(stderr, "socket error\n");
    //     return (-1);
    // }

    // int one = 1;
    // struct sockaddr_in sin;
    // sin.sin_family = AF_INET;
	// sin.sin_port = htons(8080);
	// sin.sin_addr.s_addr = inet_addr ("18.130.68.79");
	
	// // if (setsockopt (sock_fd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    // if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one)) < 0) {
    //     fprintf(stderr, "socket opt");
    //     return (-1);
	// }

	// if (sendto (sock_fd, pack, 60,	0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
	// {
    //     fprintf(stderr, "sendto");
    //     return (-1);
	// }
	// //Data send successfully
	// else
	// {
	// 	printf ("Packet Send. Length : %d \n" , 60);
    //     return (1);
	// }


    // /* Now read data coming from the kernel */
    // // READING 1 PACKAGE
    // // while (1)
    // // {
    // /* Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes */
    // // nread = cread(tun_fd, buffer, sizeof(buffer));

    // // /* Do whatever with the data */
    // // printf("Read %d from device %s\n", nread, tun_name);

    // // for (int i = 0; i < nread; i++)
    // // {
    // //     // printf("%02hhX ", buffer[i]);
    // //     printf("%d, ", buffer[i]);
    // // }
    // // printf("\n");


    // // nwrite = write(sock_fd, pack, strlen(pack));

    // // printf("Sent %d bytes from devide %s \n", nwrite, tun_name);
    // // }

    return 0;
}
