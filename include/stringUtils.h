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

void trimStart(char * destination, const char * string);
bool getSplitComponent(char * destination, const char * string, char delimiter, int index);
void joinWithDelimiter(char * destination, const char * stringA, const char * stringB, char delimiter);


#endif /* __STRING_UTILS_H__ */