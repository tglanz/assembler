#include "parsing.h"

/* The whitespaces */
const char WHITSPACES[] = { ' ', '\t', '\n' };
const int WHITSPACES_COUNT = sizeof(WHITSPACES) / sizeof(char);

DirectiveType directiveTypeFromString(string directiveString){
    /* just check what the string is, return the appropriate value */
    if (strcmp(directiveString, "data") == 0){
        return DIRECTIVE_TYPE_DATA;
    } else if (strcmp(directiveString, "string") == 0){
        return DIRECTIVE_TYPE_STRING;
    } else if (strcmp(directiveString, "entry") == 0){
        return DIRECTIVE_TYPE_ENTRY;
    } else if (strcmp(directiveString, "extern") == 0){
        return DIRECTIVE_TYPE_EXTERN;
    } else {
        return DIRECTIVE_TYPE_INVALID;
    }
}

bool isWhitespaceCharacter(char character){
    /* iterate through whitespaces, is character one of those? */
    uint idx;
    for (idx = 0; idx < WHITSPACES_COUNT; ++idx){
        if (character == WHITSPACES[idx]){
            return true;
        }
    }
    return false;
}

bool isWhitespaceLine(string line){
    /* foreach character, is it a whitespace? */
    int cursor = strlen(line);
    while (--cursor >= 0){
        if (!isWhitespaceCharacter(line[cursor])){
            return false;
        }
    }
    return true;
}

bool isAlphabeticalCharacter(char character, bool includeLowerCase, bool includeUpperCase){
    /* ascii characters are linearly mapped, check if in relevant range */
    return (includeLowerCase && character >= 'a' && character <= 'z') ||
           (includeUpperCase && character >= 'A' && character <= 'Z');
}

bool isNumericCharacter(char character){
    /* ascii characters are linearly mapped, check if in relevant range */
    return character >= '0' && character <= '9';
}

bool tryGetLabel(char * destination, string line) {
    /* a label can only exist up to a ':' character */
    return untilCharacterExclusive(destination, line, ':');
}

bool tryGetDirective(char * destination, string line){
    /* a directive is a non-empty string, from '.' character to a whitespace */
    if (!fromCharacterInclusive(destination, line, '.')){
        return false;
    }

    fromIndexInclusive(destination, destination, 1);
    untilCharacterExclusive(destination, destination, ' ');
    return true;
}

bool tryGetDirectiveArgs(char * destination, string line){
    /* directive args can appear from the next non whitespace after a directive */
    if (fromCharacterInclusive(destination, line, '.') && fromCharacterInclusive(destination, destination, ' ')){
        trimStart(destination, destination);
        trimRepeatedCharacter(destination, destination, ' ');
        return true;
    }
    return false;
}

bool tryGetOperation(char * destinationOperation, char * destinationArguments, string line, bool hasLabel){
    uint idx;
    char tmp[MAX_LINE_LENGTH];

    strcpy(tmp, line);
    for (idx = 0; idx < WHITSPACES_COUNT; ++idx){
        trimRepeatedCharacter(tmp, tmp, WHITSPACES[idx]);       
    }
    trimStart(tmp, tmp);

    /*
        operation name
            entries have the following patterns:
                1. label: operation ... -> hasLabel = true
                2. operation ...        -> hasLabel = false
    */
    if (!getSplitComponent(destinationOperation, tmp, " ", hasLabel ? 1 : 0)){
        return false;
    }

    idx = substringIndex(line, destinationOperation);
    idx += strlen(destinationOperation);
    strcpy(destinationArguments, line + idx);
    removeWhitespaces(destinationArguments, destinationArguments);
    return true;
}

bool isValidLabel(string label){
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

void removeWhitespaces(char * destination, string source) {
    uint idx;
    for (idx = 0; idx < WHITSPACES_COUNT; ++idx){
        removeCharacter(destination, source, WHITSPACES[idx]);       
    }    
}
