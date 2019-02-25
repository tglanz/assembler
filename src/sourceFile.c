#include "sourceFile.h"

bool isCommentLine(const char * line){
    return strlen(line) > 1 && line[0] == COMMENT_CHAR;
}

bool readLine(FILE * file, char * destination){
    int character;
    int idx = 0;
    while ((character = fgetc(file)) != EOF && character != '\n'){
        destination[idx] = character;
        ++idx;
    }
    destination[idx] = '\0';
    return idx != 0 || character != EOF;
}

bool isMeaningfulLine(const char * line){
    return !isWhitespaceLine(line) && !isCommentLine(line);
}

SourceFile * openSourceFile(const char * baseName){
    char filePath[MAX_FILE_PATH_LENGTH];
    joinWithDelimiter(filePath, baseName, "as", '.');
    return fopen(filePath, "r");
}