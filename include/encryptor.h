#ifndef ENCRYPTOR_H_
#define ENCRYPTOR_H_

/**
 * Encryptor module.
 * 
 * Defines interfaces for functions and constants that encrypt and convert
 * byte data to natural language.
 * 
*/

#define VOCABULARY_LEN 128 // number of possible bytes

// using byte to encode a char (1 byte of information)
typedef char byte;

/**
 * Encrypt bytes.
 * 
 * @param bytes: array of bytes to be encoded;
 * @param destination: the pointer to the destination message location;
 * 
 * @return -1 for error,1 for completed execution
*/
int encrypt(const byte* bytes, char** destination);


/**
 * Decrypt message.
 * 
 * @param input: the input message pointer to be decrypted.
 * @param output: pointer to the bytes array to save the message;
 * 
*/
int decrypt(char* input, byte** output);


/**
 * _dictionary
 * 
 * used by the interfaces to encode and decode the bytes in natural language.
*/
const char* _dictionary [VOCABULARY_LEN] = {
    "the",
    "be",
    "to",
    "of",
    "and",
    "a",
    "in",
    "that",
    "have",
    "I",
    "it",
    "for",
    "not",
    "on",
    "with",
    "he",
    "as",
    "you",
    "do",
    "at",
    "this",
    "but",
    "his",
    "by",
    "from",
    "they",
    "we",
    "say",
    "her",
    "she",
    "or",
    "an",
    "will",
    "my",
    "one",
    "all",
    "would",
    "there",
    "their",
    "what",
    "so",
    "up",
    "out",
    "if",
    "about",
    "who",
    "get",
    "which",
    "go",
    "me",
    "when",
    "make",
    "can",
    "like",
    "time",
    "no",
    "just",
    "him",
    "know",
    "take",
    "person",
    "into",
    "year",
    "your",
    "good",
    "some",
    "could",
    "them",
    "see",
    "other",
    "then",
    "now",
    "look",
    "only",
    "come",
    "its",
    "over",
    "think",
    "also",
    "back",
    "after",
    "use",
    "two",
    "how",
    "our",
    "work",
    "first",
    "well",
    "way",
    "even",
    "new",
    "want",
    "because",
    "any",
    "these",
    "give",
    "day",
    "most",
    "us",
    "is",
    "here",
    "such",
    "great",
    "old",
    "man",
    "many",
    "too",
    "before",
    "must",
    "through",
    "where",
    "much",
    "should",
    "right",
    "high",
    "different",
    "small",
    "large",
    "next",
    "early",
    "important",
    "public",
    "bad",
    "same",
    "able",
    "to",
    "of",
    "in"
};


#endif // ENCRYPTOR_H_