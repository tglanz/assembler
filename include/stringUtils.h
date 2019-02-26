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

bool getSplitComponent(char * destination, string source, string delimiters, int componentIndex);
void joinWithDelimiter(char * destination, string stringA, string stringB, char delimiter);
int substringIndex(string source, string substring);
bool substringInRange(char * destination, string source, int startInclusive, int endExclusive);
int findCharacterIndex(string source, int offset, char character);
int countCharacterOccurrences(string source, int offset, char character);
bool fromIndexInclusive(char * destination, string source, int index);
bool untilIndexExclusive(char * destination, string source, int index);
bool fromCharacterInclusive(char * destination, string source, char character);
bool untilCharacterExclusive(char * destination, string source, char character);
void trimStart(char * destination, string source);
void trimRepeatedCharacter(char * destination, string source, char character);
void removeCharacter(char * destination, string source, char character);

#endif /* __STRING_UTILS_H__ */