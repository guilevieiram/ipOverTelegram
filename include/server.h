#ifndef SERVER_H_
#define SERVER_H_
#include "telegram.h"
#include "encryptor.h"

/**
 * Server module;
 * 
 * Defines the server interface;
*/


/**
 * Main entrypoint for the server
 * 
 * Executes two functions, each on one threads: 
 * - the function that read and decode the message from telegram and write it in the tunnel
 * - the function that read and encode the message from the tunnel and send it in telegram
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
 * Listen server.
 * 
 * The server-side function that read in the tunel, encode the message and send it to telegram
*/
void * listen_server();


#endif //SERVER_H_