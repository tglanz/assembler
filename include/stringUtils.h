#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <stdlib.h>
#include <string.h>

#include "parsing.h"

/**
 * Utilities for string manipulation.
 * 
 * Emphasize on immutability, dont change inputs (like strtok etc..).
 * 
 * functions will have the form of
 *      void/bool funcName(char * destination, ...)
 * 
 * destination argument is the only argument that can be modified
 * bool return values indicate whether or not the destination is relvant for the action taken
 * 
 * Hopefuly it can spare some bugs on my part
 */
#include "stringUtils.h"

bool getSplitComponent(char * destination, const char * string, const char * delimiters, int componentIndex);
void joinWithDelimiter(char * destination, const char * stringA, const char * stringB, char delimiter);
bool substringInRange(char * destination, const char * string, int startInclusive, int endExclusive);
int findCharacterIndex(const char * string, int offset, char character);
int countCharacterOccurrences(const char * string, int offset, char character);
bool fromIndexInclusive(char * destination, const char * string, int index);
bool untilIndexExclusive(char * destination, const char * string, int index);
bool fromCharacterInclusive(char * destination, const char * string, char character);
bool untilCharacterExclusive(char * destination, const char * string, char character);
void trimStart(char * destination, const char * string);
void trimRepeatedCharacter(char * destination, const char * string, char character);
void removeCharacter(char * destination, const char * string, char character);

#endif /* __STRING_UTILS_H__ */