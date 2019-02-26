#ifndef __BASE_64_H__
#define __BASE_64_H__

#include "core.h"
#include "words.h"

#define BASE64_CAPITAL_Z_ENCODING (25)
#define BASE64_SMALL_Z_ENCODING (51)
#define BASE64_DIGIT_9_ENCODING (61)

/* word size / 2 */
#define BASE64_SINGLE_WIDTH (6)

typedef struct {
    uint raw: BASE64_SINGLE_WIDTH;
} Base64Single;

typedef union {
    Word word;
    struct {
        uint low: BASE64_SINGLE_WIDTH;
        uint high: BASE64_SINGLE_WIDTH;
    } fields;
} SplittedWord;

char singleToBase64(Base64Single single);
void wordToBase64(char * destination, Word word);

#endif /* __BASE_64_H__ */