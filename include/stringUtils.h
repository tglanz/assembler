/**
 * Provide mechanisms to manipulate and construct strings.
 * This is basically a wrapper around the standard string functionality.
 * 
 * We have put an emphasize on immutability - Which might cause performance issues, in terms
 * of computation and memory, but help in readability, robustness and less bugs (hopefuly).
 * 
 * The functions in this module will mostly have the structure
 * (return type) name(char * destination, ...inputs)
 * where all of the inputs are immutable references are passed by values, and only
 * destination will change.
 * 
 * This allows two approaches of use - 
 * 1. manipulate into new result string for examle
 *      char destination[10];
 *      char source[] = "abc";
 *      fromIndexInclusive(destination, source, 2);
 *      --> will result in `destination` == "c";
 * 2. manipulate into same string for example
 *      char str[] = "abc";
 *      fromIndexInclusive(str, str, 2);
 *      --> will result in `str` == "c";
 *
 * But as a rule, inputs will alway have a const specifier to enforce those methodologies.
 */
#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <stdlib.h>
#include <string.h>

#include "parsing.h"

#include "stringUtils.h"

/**
 * Split a given string into components according to delimiters, and will provide a specific one
 * 
 * Arguments
 *      destination: char* - pointer to store the result at
 *      source: string - the input string
 *      delimiters: string - delimiters to split by
 *      componentIndex: int - the index of the component to get
 * 
 * Examples
 * 
 *      1. char destination[10];
 *         getSplitComponent(destination, "a.b", ",", 0)
 *         will yield -> `destination` == "a"
 * 
 *      2. char destination[10];
 *         getSplitComponent(destination, "a.b", ",", 1)
 *         will yield -> `destination` == "b"
 * 
 * Returns
 *      bool - true if found a relevant component according to the index and destination will contain
 *          a valid string
 */
bool getSplitComponent(char * destination, string source, string delimiters, int componentIndex);

/**
 * Joins two strings, with a delimiter in between
 * 
 * Arguments
 *      destination: char* - pointer to store the result at
 *      stringA: string - the left string
 *      stringB: string - the right string
 *      delimiter: char - the delimiter
 */
void joinWithDelimiter(char * destination, string stringA, string stringB, char delimiter);

/**
 * Retrieves the index of the first ocurrence of a specifed substring
 * 
 * Examples
 *      1. substringIndex("abcdef", "cd") --> 2
 *      2. substringIndex("ababab", "bab") --> 2 (and not 3)
 *      3. substringIndex("ababab", "notfound") --> -1
 * 
 * Arguments
 *      source: string - the string to read
 *      substring: string - the substring to find inside source
 * 
 * Returns
 *      int - the index of the substring.
 *          -1 if not found
 */
int substringIndex(string source, string substring);

/**
 * Extract a substring in the given range from the given string
 * 
 * Example
 *      char destination[10];
 *      substringInRange(destination, "abcde", 1, 3)
 *      will yield --> `destination` == "bc"
 * 
 * Arguments
 *      destination: char* - pointer to store the results at
 *      source: string - string to extract a substring from
 *      startInclusive: int - the substring starting index (inclusive)
 *      endExlusive: int - the substring end index (exclusive)
 * 
 * Returns
 *      bool - true if a substring was extracted and destination contain a valid string
 */
bool substringInRange(char * destination, string source, int startInclusive, int endExclusive);

/**
 * Find the index of the first ocurrence of a character in a string
 * 
 * Example
 *      1. findCharacterIndex("abcdef", 0, 'c') --> 2
 *      2. findCharacterIndex("abcabc", 0, 'c') --> 2
 *      3. findCharacterIndex("abcabc", 3, 'c') --> 5
 *      4. findCharacterIndex("abcdef", 0, 'z') --> -1
 * 
 * Arguments
 *      source: string - string to get the index from
 *      offset: int - and index to start searching from
 *      character: char - the character to look for
 * 
 * Returns
 *      int - the index of the character in the string
 *          -1 if not found
 */
int findCharacterIndex(string source, int offset, char character);

/**
 * Count the number of ocurrences of a given character in a given string
 * 
 * Examples
 *      1. countCharacterOccurrences("aaa", 0, 'a') --> 3
 *      2. countCharacterOccurrences("aaa", 1, 'a') --> 2
 *      3. countCharacterOccurrences("aaa", 0, 'z') --> 0
 * 
 * Arguments
 *      source: string - the string to count the character in
 *      offset: int - an index to start scanning from
 *      character: char - the character to count ocurrences of
 * 
 * Returns
 *      int - the number of ocurrences (0 if none)
 */
int countCharacterOccurrences(string source, int offset, char character);

/**
 * For convenient - 
 * Same as substringInRange(destination, source, index, infinity)
 */
bool fromIndexInclusive(char * destination, string source, int index);

/**
 * For convenient - 
 * Same as substringInRange(destination, source, 0, index)
 */
bool untilIndexExclusive(char * destination, string source, int index);

/**
 * For convenient - 
 * Same as fromIndexInclusive(destination, source,
 *                            findCharacterIndex(source, 0, character))
 */
bool fromCharacterInclusive(char * destination, string source, char character);

/**
 * For convenient - 
 * Same as untilIndexExclusive(destination, source,
 *                            findCharacterIndex(source, 0, character))
 */
bool untilCharacterExclusive(char * destination, string source, char character);

/**
 * Removes all whitespace characters from the start of a string
 * 
 * Examples
 *      1. char destination[10];
 *         trimStart(destination, "   a")
 *         will yield --> `destination` == "a"
 *      2. char destination[10];
 *         trimStart(destination, "   a    b")
 *         will yield --> `destination` == "a    b"
 * 
 * Arguments
 *      destination: char* - pointer to store the result at
 *      source: string - string to remove whitespaces from
 */
void trimStart(char * destination, string source);

/**
 * Given a target character, removes and consequtive apperances of it
 * 
 * Examples
 *      1. char destination[10];
 *         trimRepeatedCharacter(destination, "abbccc", 'b')
 *         will yield --> `destination` == "abccc"
 * 
 * Arguments
 *      destination: char* - pointer to store the result at
 *      source: string - string to operate on
 *      character: char - the target character
 */
void trimRepeatedCharacter(char * destination, string source, char character);

/**
 * Completely remove all ocurrences of a given character
 * 
 * Examples
 *      1. char destination[10];
 *         removeCharacter(destination, "abbcbcbbbcbbb", 'b')
 *         will yield --> `destination` == "accc"
 * 
 * Arguments
 *      destination: char* - pointer to store the result at
 *      source: string - string to remove the character from
 *      character: char - the character to remove
 */
void removeCharacter(char * destination, string source, char character);

#endif /* __STRING_UTILS_H__ */