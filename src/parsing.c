#include "parsing.h"

bool isWhitespaceCharacter(char character){
    return character == ' ' || character == '\t' || character == '\n';
}

bool isWhitespaceLine(const char * line){
    int cursor = strlen(line);
    while (--cursor >= 0){
        if (!isWhitespaceCharacter(line[cursor])){
            return false;
        }
    }
    return true;
}

bool isAlphabeticalCharacter(char character, bool includeLowerCase, bool includeUpperCase){
    if (includeLowerCase && character >= 'a' && character <= 'z')
        return true;
    if (includeUpperCase && character > 'A' && character <= 'Z')
        return true;
    return false;
}

bool isNumericCharacter(char character){
    return character >= '0' && character <= '9';
}

DirectiveType directiveTypeFromString(const char * string){
    if (strcmp(string, "data") == 0){
        return DIRECTIVE_TYPE_DATA;
    } else if (strcmp(string, "string") == 0){
        return DIRECTIVE_TYPE_STRING;
    } else if (strcmp(string, "entry") == 0){
        return DIRECTIVE_TYPE_ENTRY;
    } else if (strcmp(string, "extern") == 0){
        return DIRECTIVE_TYPE_EXTERN;
    } else {
        return DIRECTIVE_TYPE_INVALID;
    }
}

bool tryGetLabel(char * destination, const char * line) {
    return getSplitComponent(destination, line, ':', 0);
}

bool tryGetDirective(char * destination, const char * line){
    char tmp[MAX_LINE_LENGTH];
    if (getSplitComponent(tmp, line, '.', 1)){
        if (getSplitComponent(destination, tmp, ' ', 0)){
            return true;
        }
    }
    return false;
}

bool isValidLabel(const char * label){
    char character;
    int size = strlen(label);

    /* length condition */
    if (size > MAX_LABEL_LENGTH){
        return false;
    }

    /* label cannot be as a directive */
    if (directiveTypeFromString(label) != DIRECTIVE_TYPE_INVALID){
        return false;
    }

    // TODO: check if as operation

    /* for all characters except the first */
    while (--size > 0){
        character = label[size];
        if (!isAlphabeticalCharacter(character, true, true) && !isNumericCharacter(character)){
            return false;
        }
    }

    /* check the first character */
    return isAlphabeticalCharacter(label[0], true, true);
}