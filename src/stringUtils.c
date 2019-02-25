#include "stringUtils.h"

bool getSplitComponent(char * destination, const char * string, const char * delimiters, int componentIndex){
    char *source, *component;
    source = strdup(string);

    component = strtok(source, delimiters);

    while (component != NULL && componentIndex > 0){
        component = strtok(NULL, delimiters);
        --componentIndex;
    }

    destination[0] = '\0';
    if (component != NULL){
        strcpy(destination, component);
    }

    free(source);
    return destination[0] != '\0';
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

int substringIndex(const char * string, const char * substring) {
    char * ptrToSubstring = strstr(string, substring);
    if (ptrToSubstring == NULL){
        return -1;
    }
    return ptrToSubstring - string;

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
    int index = findCharacterIndex(string, 0, character);
    return index >= 0 && fromIndexInclusive(destination, string, index);
}

bool untilCharacterExclusive(char * destination, const char * string, char character){
    int index = findCharacterIndex(string, 0, character);
    return index >= 0 && untilIndexExclusive(destination, string, index);
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