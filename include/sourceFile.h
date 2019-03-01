/**
 * Functionalities and definitions, regarding source files
 */
#ifndef  __SOURCE_FILE_H__
#define __SOURCE_FILE_H__

/* maximum length of the file path */
#define MAX_FILE_PATH_LENGTH (256)

/* maximum length of a line in the source file */
#define MAX_LINE_LENGTH (80 + 1)

/* a character representnig a comment */
#define COMMENT_CHAR (';')

/* standard */
#include <stdlib.h>
#include <stdio.h>

/* services */
#include "core.h"
#include "parsing.h"
#include "stringUtils.h"

/* Alias the source file, can be used to extend it without breaking code */
typedef FILE SourceFile;

/**
 * Opens a source file
 * 
 * Arguments
 *      baseName: string - the base name of the path of the file
 * 
 * Returns
 *      SourceFile* - a reference to the opened file, or NULL if failed opening
 */
SourceFile * openSourceFile(string baseName);

/**
 * Reads the next line from a given file
 * 
 * The operation will move the file cursor to point to the next line
 * 
 * Arguments
 *      file: SourceFile* - the file to read the line from
 *      destination: char* - a pointer to store the line at
 * 
 * Returns
 *      bool - indicates whether a line is read.
 *          true if destination contains a new, valid line
 *          false if reached eof
 */
bool readLine(SourceFile * file, char * destination);

/**
 * Determine whether a given line has any meaning, and should be parsed
 * 
 * Arguments
 *      line: string - the line to check
 * 
 * Returns
 *      bool - true if the line should be parsed
 */
bool isMeaningfulLine(string line);

#endif /* __SOURCE_FILE_H__ */