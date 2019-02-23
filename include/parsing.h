#ifndef __PARSING_H__
#define __PARSING_H__

/**
 * Functions which analyze inputs with respect to our domain
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "core.h"
#include "sourceFile.h"
#include "stringUtils.h"

#define MAX_LABEL_LENGTH (31)

typedef enum {
    DIRECTIVE_TYPE_DATA,
    DIRECTIVE_TYPE_STRING,
    DIRECTIVE_TYPE_ENTRY,
    DIRECTIVE_TYPE_EXTERN,
    DIRECTIVE_TYPE_INVALID
} DirectiveType;

DirectiveType directiveTypeFromString(const char * string);
const char * stringFromDirectiveType(DirectiveType directiveType);

bool isWhitespaceCharacter(char character);
bool isWhitespaceLine(const char * line);
bool isAlphabeticalCharacter(char character, bool includeLowerCase, bool includeUpperCase);
bool isNumericCharacter(char character);

bool tryGetLabel(char * destination, const char * line);
bool tryGetDirective(char * destination, const char * line);
bool tryGetDirectiveArgs(char * destination, const char * line);
bool tryGetOperation(char * destinationOperation, char * destinatinoArguments, const char * line, bool hasLabel);

bool isValidLabel(const char * label);

#endif /* __PARSING_H__ */