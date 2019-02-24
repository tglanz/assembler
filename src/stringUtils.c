#include "stringUtils.h"

bool getSplitComponent(char * destination, const char * string, char delimiter, int componentIndex){
    int idx, cursor, length;
    bool hasComponent;

    hasComponent = false;
    cursor = 0;
    length = strlen(string);

    for (idx = 0; idx < length; ++idx){
        if (componentIndex == 0){
            if (string[idx] == delimiter){
                hasComponent = true;
                break;
            }

            destination[cursor++] = string[idx];
        } else if (string[idx] == delimiter){
            --componentIndex;
            if (componentIndex == 0 && idx < length - 1){
                hasComponent = true;
            }
        }
    }

    destination[cursor] = '\0';
    return hasComponent;
}

void joinWithDelimiter(char * destination, const char * stringA, const char * stringB, char delimiter){
    char delimiterString[1];
    delimiterString[0] = delimiter;

    strcpy(destination, stringA);
    destination += strlen(stringA);

    strcpy(destination, delimiterString);
    destination += 1;

    strcpy(destination, stringB);
    destination += strlen(stringB);

    destination[0] = '\0';
}

bool substringInRange(char * destination, const char * string, int startInclusive, int endExclusive) {
    int cursor = 0;

    if (startInclusive >= endExclusive){
        return false;
    }

    while (startInclusive + cursor < endExclusive){
        destination[cursor] = string[startInclusive + cursor];
        ++cursor;
    }

    destination[cursor] = '\0';
    return true;
}

int findCharacterIndex(const char * string, int offset, char character){
    while (offset < strlen(string)){
        if (string[offset] == character){
            return offset;
        }
        ++offset;
    }

    return -1;
}

int countCharacterOccurrences(const char * string, int offset, char character) {
    int count = 0;
    while (offset < strlen(string)){
        if (string[offset] == character){
            ++count;
        }
        ++offset;
    }
    return count;
}

bool fromIndexInclusive(char * destination, const char * string, int index){
    return substringInRange(destination, string, index, strlen(string));
}

bool untilIndexExclusive(char * destination, const char * string, int index){
    return substringInRange(destination, string, 0, index);
}

bool fromCharacterInclusive(char * destination, const char * string, char character){
    return fromIndexInclusive(destination, string, findCharacterIndex(string, 0, character));
}

bool untilCharacterExclusive(char * destination, const char * string, char character){
    return untilIndexExclusive(destination, string, findCharacterIndex(string, 0, character));
}

void trimStart(char * destination, const char * string){
    int idx = 0;
    while (isWhitespaceCharacter(string[idx])){
        ++idx;
    }

    fromIndexInclusive(destination, string, idx);
}

void trimRepeatedCharacter(char * destination, const char * string, char character){
    int idx;

    for (idx = 0; idx < strlen(string); ++idx){
        if (idx == 0 || string[idx] != character || destination[-1] != character){
            destination[0] = string[idx];
            destination += 1;
        }
    }

    destination[0] = '\0';
}

void removeCharacter(char * destination, const char * string, char character){
    int idx;

    for (idx = 0; idx < strlen(string); ++idx){
        if (string[idx] != character){
            destination[0] = string[idx];
            destination += 1;
        }
    }

    destination[0] = '\0';
}