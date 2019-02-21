#include "parsing.h"

bool readLine(FILE * file, char * buffer){
    int character;
    while ((character = fgetc(file)) != EOF && character != '\n'){
        *buffer = character;
        buffer += 1;
    }
    *buffer = '\0';
    return character != EOF;
}

bool isEmptyCharacter(char character){
    return character == ' ' || character == '\t' || character == '\n';
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

bool tryGetLabel(char * line, char * buffer) {
    return tryGetSplitComponent(line, ':', 0, buffer);
}

bool tryGetDirective(char * line, char * buffer){
    char tmp[MAX_LINE_LENGTH];
    if (tryGetSplitComponent(line, '.', 1, tmp)){
        if (tryGetSplitComponent(tmp, ' ', 0, buffer)){
            return true;
        }
    }
    return false;
}

char * withoutWhitespace(char * string){
    int idx = 0;
    while (string != NULL && isEmptyCharacter(string[idx]))
        ++idx;
    return string + idx;
}

bool tryGetSplitComponent(const char * string, char delimiter, int componentIndex, char * buffer){
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

            buffer[cursor++] = string[idx];
            buffer[cursor] = '\0';
        } else if (string[idx] == delimiter){
            --componentIndex;
            if (componentIndex == 0 && idx < length - 1){
                hasComponent = true;
            }
        }
    }

    return hasComponent;
}

bool isEmptyLine(char * line){
    int cursor = strlen(line);
    while (--cursor >= 0){
        if (!isEmptyCharacter(line[cursor])){
            return false;
        }
    }
    return true;
}

bool isCommentLine(char * line){
    return strlen(line) > 1 && line[0] == COMMENT_CHAR;
}
