/**
 * Define functions to parse strings, and analyze them,
 * specificaly according to our domain
 */
#ifndef __PARSING_H__
#define __PARSING_H__

/* standard */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* services */
#include "core.h"
#include "sourceFile.h"
#include "stringUtils.h"

/* Maximum size of a label string */
#define MAX_LABEL_LENGTH (31)

/* Parsable directive types */
typedef enum {
    DIRECTIVE_TYPE_DATA,
    DIRECTIVE_TYPE_STRING,
    DIRECTIVE_TYPE_ENTRY,
    DIRECTIVE_TYPE_EXTERN,
    DIRECTIVE_TYPE_INVALID
} DirectiveType;

/**
 * Parse a string of a directive
 * 
 * Arguments
 *      directiveString: string - a directive string
 * 
 * Examples
 *      ".entry" -> DIRECTIVE_TYPE_ENTRY
 *      "entry" -> DIRECTIVE_TYPE_INVALID
 * 
 * Returns
 *      DirectiveType - the parsed directive
*              DIRECTIVE_TYPE_INVALID if could not be parsed succesfully
 */
DirectiveType directiveTypeFromString(string directiveString);

/**
 * Checks whether a character is whitepace
 * 
 * Arguments
 *      character: char - the character to check
 * 
 * Returns
 *      bool - true if character is whitespace
 */
bool isWhitespaceCharacter(char character);

/**
 * Checks whether a line contain only whitespace characters
 * 
 * Arguments
 *      line: string - the line to check
 * 
 * Returns
 *      bool - true if line contain only whitepsace characters
 */
bool isWhitespaceLine(string line);

/**
 * Checks whether a character is alphabetical
 * 
 * Arguments
 *      character: char - the character to check
 *      incudeLowerCase: bool - check lower case characters
 *      incudeUpperCase: bool - check upper case characters
 * 
 * Example
 *      ('1', _, _) -> false
 *      ('a', false, true) -> false
 *      ('a', true, false) -> true
 * 
 * Returns
 *      bool - true if character is alphabetical
 */
bool isAlphabeticalCharacter(char character, bool includeLowerCase, bool includeUpperCase);

/**
 * Checks whether a character is numeric
 * 
 * Arguments
 *      character: char - the character to check
 * 
 * Example
 *      'a' -> false
 *      '2' -> true
 * 
 * Returns
 *      bool - true if character is numeric
 */
bool isNumericCharacter(char character);

/**
 * Tries to parse and extract a label from a given string
 * This is done according to ower specification
 * 
 * Arguments
 *      destination: char* - pointer to store the label at
 *      line: string - the line to extract the label from
 * 
 * Returns
 *      true - found a label, destination will store a valid string
 */
bool tryGetLabel(char * destination, string line);

/**
 * Tries to parse and extract a directive from a given string
 * This is done according to ower specification
 * 
 * Arguments
 *      destination: char* - pointer to store the directive at
 *      line: string - the line to extract the label from
 * 
 * Returns
 *      true - found a directive, destination will store a valid string
 */
bool tryGetDirective(char * destination, string line);

/**
 * Tries to parse and extract the arguments of a directive from a given string
 * This is done according to ower specification
 * 
 * Arguments
 *      destination: char* - pointer to store the args at
 *      line: string - the line to extract the label from
 * 
 * Returns
 *      true - found a args, destination will store a valid string
 */
bool tryGetDirectiveArgs(char * destination, string line);

/**
 * Tries to parse and extract the operation from a given string
 * This is done according to ower specification
 * 
 * Arguments
 *      destinationOperation: char* - pointer to store the operation at
 *      destinationArguments: char* - pointer to store the arguments at
 *      line: string - the line to extract the label from
 *      hasLabel: bool - indicates whether the line has label in it
 * 
 * Returns
 *      true - found an operation, destination will store a valid string
 */
bool tryGetOperation(char * destinationOperation, char * destinationArguments, string line, bool hasLabel);

/**
 * Determines whether a given string is a valid label
 * 
 * Examples
 *      "SOMETHING" -> true
 *      "SOMETHING:" -> false
 *      "    SOMETHING" -> false
 * 
 * Arguments
 *      label: string - the string to check
 * 
 * Returns
 *      bool - true if the string is a valid label
 */
bool isValidLabel(string label);

/**
 * Remove whitepsaces from a given string
 * 
 * Arguments
 *      destination: char* - a pointer to store the result at
 *      source: string - a string to remove the whitespaces from
 */
void removeWhitespaces(char * destination, string source);

#endif /* __PARSING_H__ */