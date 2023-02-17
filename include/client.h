#ifndef CLIENT_H_
#define CLIENT_H_
#include "telegram.h"
#include "encryptor.h"

/**
 * Client module
 * 
 * Defines the client interface
*/


/**
 * Main entrypoint for the client
 * 
 * Executes two functions, each on one threads: 
 * - the function that read and encode the message from the tunnel and send it in telegram
 * - the function that read and decode the message from telegram and write it in the tunnel
 * 
 *  @return: -1 for errors; 1 for completed execution; 
*/
int main();

/**
 * Process message.
 * 
 * This functions decode a package and write it in the tunnel
 * 
 * @param message: the message to be decoded and writen in the tunnel
 * @param args: the additional argument(s) to be passed
*/
void process_message(char* message, const void* arg);

/**
 * Receive client.
 * 
 * The client-side function that read telegram message, decode it and write it in the tunnel
*/
void * receive_client();

#endif //CLIENT_H_