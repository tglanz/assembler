#include "stringUtils.h"

char * strdup(string source){
    char * duplicated = (char*)malloc(strlen(source) + 1);
    strcpy(duplicated, source);
    return duplicated;
}

bool getSplitComponent(char * destination, string source, string delimiters, int componentIndex){
    char *duplicated, *component;
    duplicated = strdup(source);

    component = strtok(duplicated, delimiters);

    while (component != NULL && componentIndex > 0){
        component = strtok(NULL, delimiters);
        --componentIndex;
    }

    destination[0] = '\0';
    if (component != NULL){
        strcpy(destination, component);
    }

    free(duplicated);
    return destination[0] != '\0';
}

void joinWithDelimiter(char * destination, string stringA, string stringB, char delimiter){
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

int substringIndex(string source, string substring) {
    char * ptrToSubstring = strstr(source, substring);
    if (ptrToSubstring == NULL){
        return -1;
    }
    return ptrToSubstring - source;

}

bool substringInRange(char * destination, string source, int startInclusive, int endExclusive) {
    int cursor = 0;

    if (startInclusive >= endExclusive){
        return false;
    }

    while (startInclusive + cursor < endExclusive){
        destination[cursor] = source[startInclusive + cursor];
        ++cursor;
    }

    destination[cursor] = '\0';
    return true;
}

int findCharacterIndex(string source, int offset, char character){
    while (offset < strlen(source)){
        if (source[offset] == character){
            return offset;
        }
        ++offset;
    }

    return -1;
}

int countCharacterOccurrences(string source, int offset, char character) {
    int count = 0;
    while (offset < strlen(source)){
        if (source[offset] == character){
            ++count;
        }
        ++offset;
    }
    return count;
}

bool fromIndexInclusive(char * destination, string source, int index){
    return substringInRange(destination, source, index, strlen(source));
}

bool untilIndexExclusive(char * destination, string source, int index){
    return substringInRange(destination, source, 0, index);
}

bool fromCharacterInclusive(char * destination, string source, char character){
    int index = findCharacterIndex(source, 0, character);
    return index >= 0 && fromIndexInclusive(destination, source, index);
}

bool untilCharacterExclusive(char * destination, string source, char character){
    int index = findCharacterIndex(source, 0, character);
    return index >= 0 && untilIndexExclusive(destination, source, index);
}

void trimStart(char * destination, string source){
    int idx = 0;
    while (isWhitespaceCharacter(source[idx])){
        ++idx;
    }

    fromIndexInclusive(destination, source, idx);
}

void trimRepeatedCharacter(char * destination, string source, char character){
    int idx;

    for (idx = 0; idx < strlen(source); ++idx){
        if (idx == 0 || source[idx] != character || destination[-1] != character){
            destination[0] = source[idx];
            destination += 1;
        }
    }

    destination[0] = '\0';
}

void removeCharacter(char * destination, string source, char character){
    int idx;

    for (idx = 0; idx < strlen(source); ++idx){
        if (source[idx] != character){
            destination[0] = source[idx];
            destination += 1;
        }
    }

    destination[0] = '\0';
}
