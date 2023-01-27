#ifndef TELEGRAM_H_
#define TELEGRAM_H_

/**
 * Telegram bot interface.
 * 
 * Abstracts and centralizes the functionalities  provided by the telegram bot api.
 * Refer to each function documentation for details.
 *
*/

struct config_t {
    char* local_ip; // string storing the local ip address to be refereed by the bot. 
    char* bot_id; // bot identifier. Provided by telegram
    char* chat_id; // chat identifier. Provided by telegram
    int* frequency; // number of requests per second to check for incoming messages
    
};
typedef struct config_t config_t;


/**
 * Setup function.
 * 
 * @param config: configuration object for the bot;
 * 
 * @return: -1 in case of any error 1 in completed execution
*/
int setup(const config_t* config);


/**
 * Read posts.
 * 
 * This functions listens to responses, and thus it stops the execution of the thread its in.
 * Recommended usage in a separate thread 
 * 
 * @param process_response: function that takes the message string as the first argument
 * and any aditional arguments passed as void* args
 * @param args: the additional argument(s) to be passed to the process_response function.
 * @param config: the bot configuration
 * 
 * @return: -1 for errors; 1 for completed execution; 
*/
int read_posts(const void process_response(char *, const void *), const void* args, const config_t* config);


/**
 * Send a message.
 * 
 * @param message: the message to be sent to by the bot
 * @param config: bot configuration.
 * 
 * @return: -1 for errors; 1 for completed execution; 
*/
int send_message(const char* message, const config_t* config);


#endif //TELEGRAM_H_