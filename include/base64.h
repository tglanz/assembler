/**
 * Definitions for base 64 encoding.
 */
#ifndef __BASE_64_H__
#define __BASE_64_H__

#include "core.h"
#include "words.h"

/**
 * Some references to the encoding values.
 * In the implementation, we will use those to map ascii to base64
 */
#define BASE64_CAPITAL_Z_ENCODING (25)
#define BASE64_SMALL_Z_ENCODING (51)
#define BASE64_DIGIT_9_ENCODING (61)

/* word size / 2 */
#define BASE64_SINGLE_WIDTH (6)

/**
 * Define a type of the same size of a single value to encode to a single character
 * - i.e 6 bits wide
 */
typedef struct {
    uint raw: BASE64_SINGLE_WIDTH;
} Base64Single;

/**
 * Define a type, that matches in size to a word, and split it to 2 fields.
 * 
 * It is useful to us because a word is 12 bits, so it can be splitted evenly to 2 values
 * to encode to base 64.
 * 
 * Using this kind of approach, we could avoid using bitwise logics, such as
 *      - low = word.raw & 0x3f
 *      - high = (word.raw >> 6) & 0x3f
 * 
 * This is probably a subjective issue, but i find it nice to encode the sizes into our type system.
 */
typedef union {
    Word word;
    struct {
        uint low: BASE64_SINGLE_WIDTH;
        uint high: BASE64_SINGLE_WIDTH;
    } fields;
} SplittedWord;

/**
 * Encodes a value to a base 64 character
 * 
 * Arguments
 *      single: Base64Single - the value to encode
 * 
 * Returns
 *      char: base 64 encoded character
 */
char singleToBase64(Base64Single single);

/**
 * Encodes a value to a base 64 character
 * 
 * Arguments
 *      destination: char* - a pointer to store 2 base 64 encoded characters
 *      word: Word - a word to encode to base 64.
 *                   a word is 12bit wide, so the encoding will contain 2 characters
 * 
 * Returns
 *      char: base 64 encoded character
 */
void wordToBase64(char * destination, Word word);

#endif /* __BASE_64_H__ */