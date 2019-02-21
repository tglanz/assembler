#ifndef __PARSING_H__
#define __PARSING_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "core.h"

typedef enum {
    DIRECTIVE_TYPE_DATA,
    DIRECTIVE_TYPE_STRING,
    DIRECTIVE_TYPE_ENTRY,
    DIRECTIVE_TYPE_EXTERN,
    DIRECTIVE_TYPE_INVALID
} DirectiveType;

bool readLine(FILE * file, char * buffer);

char * withoutWhitespace(char * string);
bool tryGetSplitComponent(const char * string, char delimiter, int index, char * buffer);

DirectiveType directiveTypeFromString(const char * string);

bool isEmptyLine(char * line);
bool isCommentLine(char * line);

bool tryGetLabel(char * line, char * buffer);
bool tryGetDirective(char * line, char * buffer);

#endif /* __PARSING_H__ */