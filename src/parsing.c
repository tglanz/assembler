#include "parsing.h"

const char WHITSPACES[] = { ' ', '\t', '\n' };
const int WHITSPACES_COUNT = sizeof(WHITSPACES) / sizeof(char);

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

const char * directiveStringFromType(DirectiveType type){
    switch (type){
        case DIRECTIVE_TYPE_DATA:
            return "data";
        case DIRECTIVE_TYPE_ENTRY:
            return "entry";
        case DIRECTIVE_TYPE_EXTERN:
            return "extern";
        case DIRECTIVE_TYPE_STRING:
            return "string";
        default:
            return "invalid";
    }
}

bool isWhitespaceCharacter(char character){
    int idx;
    for (idx = 0; idx < WHITSPACES_COUNT; ++idx){
        if (character == WHITSPACES[idx]){
            return true;
        }
    }
    return false;
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
    return (includeLowerCase && character >= 'a' && character <= 'z') ||
           (includeUpperCase && character >= 'A' && character <= 'Z');
}

bool isNumericCharacter(char character){
    return character >= '0' && character <= '9';
}

bool tryGetLabel(char * destination, const char * line) {
    if (untilCharacterExclusive(destination, line, ':')){
        return true;
    }
    return false;
}

bool tryGetDirective(char * destination, const char * line){
    return fromCharacterInclusive(destination, line, '.') &&
           untilCharacterExclusive(destination, destination, ' ') &&
           fromIndexInclusive(destination, destination, 1);
}

bool tryGetDirectiveArgs(char * destination, const char * line){
    if (fromCharacterInclusive(destination, line, '.') && fromCharacterInclusive(destination, destination, ' ')){
        trimStart(destination, destination);
        trimRepeatedCharacter(destination, destination, ' ');
        return true;
    }
    return false;
}

bool tryGetOperation(char * destinationOperation, char * destinationArguments, const char * line, bool hasLabel){
    int idx;
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

    /* get the arguments in a normalized manner (no whitespces) */
    fromCharacterInclusive(destinationArguments, tmp, ' ');
    trimStart(destinationArguments, destinationArguments);
    if (hasLabel){
        fromCharacterInclusive(destinationArguments, destinationArguments, ' ');
    }
    fromIndexInclusive(destinationArguments, destinationArguments, 1);
    removeCharacter(destinationArguments, destinationArguments, ' ');
    return true;
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

    /* TODO: check if as operation */

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