#include "stringUtils.h"

void trimStart(char * destination, const char * string){
    int cursor, idx;
    bool encounteredNonWhitespace;

    encounteredNonWhitespace = false;
    cursor = 0;

    for (idx = 0; idx < strlen(string); ++idx){
        if (!isWhitespaceCharacter(string[idx])){
            encounteredNonWhitespace = true;
        }

        if (encounteredNonWhitespace){
            destination[cursor++] = string[idx];
            destination[cursor] = '\0';
        }
    }

    destination[cursor] = '\0';
}

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