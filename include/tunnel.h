#ifndef TUNNEL_H_
#define TUNNEL_H_

/**
 * TUNNEL interface
 * 
 * declare functions that allows to setup tunnels easily.
 *
 * based on the tutorial https://backreference.org/2010/03/26/tuntap-interface-tutorial/
*/


/**
 * Allocate a tunnel
 * 
 * @param dev: the name of an interface (or '\0'). MUST have enough
 * space to hold the interface name if '\0' is passed
 * @param flags: interface flags (eg, IFF_TUN etc.)
 * 
 * @return: file descriptor for the tunnel.
*/
int tun_alloc(char *dev, int flags);


#endif //TUNNEL_H_