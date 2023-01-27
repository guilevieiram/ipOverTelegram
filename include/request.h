#ifndef REQUEST_H_
#define REQUEST_H_

/**
 * Request interface
 * 
 * declare functions that allow http requests easily.
 *
*/


/**
 * get
 * 
 * @param url: url to make the request.
 * 
 * @return: string with the response.
*/
char* get(const char* url);



#endif //REQUEST_H_