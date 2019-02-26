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

DirectiveType directiveTypeFromString(string directiveString);

bool isWhitespaceCharacter(char character);
bool isWhitespaceLine(string line);
bool isAlphabeticalCharacter(char character, bool includeLowerCase, bool includeUpperCase);
bool isNumericCharacter(char character);

bool tryGetLabel(char * destination, string line);
bool tryGetDirective(char * destination, string line);
bool tryGetDirectiveArgs(char * destination, string line);
bool tryGetOperation(char * destinationOperation, char * destinatinoArguments, string line, bool hasLabel);

bool isValidLabel(string label);

void removeWhitespaces(char * destination, string source);

#endif /* __PARSING_H__ */