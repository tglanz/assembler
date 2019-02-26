#include "base64.h"

char singleToBase64(Base64Single single) {

    /* the space [0, BASE64_CAPITAL_Z_ENCODING], corresponds to the ascii space of [A, Z] */
    if (single.raw <= BASE64_CAPITAL_Z_ENCODING){
        return single.raw + 'A';
    }

    if (single.raw <= BASE64_SMALL_Z_ENCODING){
        return single.raw - BASE64_CAPITAL_Z_ENCODING - 1 + 'a';
    }

    /* the space [51, 61] correspnds to the ascii space of [0, 9] */
    if (single.raw <= BASE64_DIGIT_9_ENCODING){
        return single.raw - BASE64_SMALL_Z_ENCODING - 1 + '0';
    }
    
    /* two cases left, 62 and 63 */
    if (single.raw == BASE64_DIGIT_9_ENCODING + 1){
        return '+';
    } else {
        return '/';
    }
}

void wordToBase64(char * destination, Word word) {
    /**
     * prefered to implement using concrete types/bitfields with known sizes at compile time.
     * 
     * rather than passing lets say a uint, than to take the low bits perform a mask with 0x7
     * and the same for high bits.
     */
    SplittedWord splitted;
    Base64Single single;

    splitted.word = word;

    single.raw = splitted.fields.high;
    destination[0] = singleToBase64(single);

    single.raw = splitted.fields.low;
    destination[1] = singleToBase64(single);
}