#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <ctype.h>
#include <string.h>

#include "core.h"
#include "log.h"
#include "parsing.h"
#include "symbols.h"

typedef struct {
    unsigned int IC;
    unsigned int DC;

    bool hasError;
} State;

typedef enum {
    DIRECTIVE_TYPE_DATA,
    DIRECTIVE_TYPE_STRING,
    DIRECTIVE_TYPE_ENTRY,
    DIRECTIVE_TYPE_EXTERN,
    DIRECTIVE_TYPE_INVALID
} DirectiveType;

void assembleInput(const char * baseName);

#endif /* __ASSEMBLER_H__ */
