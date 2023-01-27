#ifndef ENCRYPTOR_H_
#define ENCRYPTOR_H_

/**
 * Encryptor module.
 * 
 * Defines interfaces for functions and constants that encrypt and convert
 * byte data to natural language.
 * 
*/

#define VOCABULARY_LEN 16 // number of possible bytes

// using byte to encode a char (1 byte of information)
typedef char byte;


/**
 * _dictionary
 * 
 * used by the interfaces to encode and decode the bytes in natural language.
*/
const char* _dictionary [VOCABULARY_LEN]{
    "the",
    "of",
    "and",
    "a",
    "to",
    "in",
    "is",
    "you",
    "that",
    "it",
    "he",
    "was",
    "for",
    "on",
    "are",
    "as"
};


/**
 * Encrypt bytes.
 * 
 * @param bytes: array of bytes to be encoded;
 * @param destination: the pointer to the destination message location;
 * 
 * @return -1 for error, 1 for completed execution
*/
int encrypt(const byte* bytes, char** destination);


/**
 * Decrypt message.
 * 
 * @param input: the input message pointer to be decrypted.
 * @param output: pointer to the bytes array to save the message;
 * 
*/
int decrypt(const char* input, byte** output);


#endif // ENCRYPTOR_H_