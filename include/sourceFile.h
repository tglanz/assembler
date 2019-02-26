#ifndef  __SOURCE_FILE_H__
#define __SOURCE_FILE_H__

#define MAX_FILE_PATH_LENGTH (256)
#define MAX_LINE_LENGTH (80 + 1)
#define COMMENT_CHAR (';')

#include <stdlib.h>
#include <stdio.h>

#include "core.h"
#include "parsing.h"
#include "stringUtils.h"

typedef FILE SourceFile;

SourceFile * openSourceFile(string baseName);
bool readLine(SourceFile * file, char * destination);
bool isMeaningfulLine(string line);

#endif /* __SOURCE_FILE_H__ */