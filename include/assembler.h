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

    SymbolsVector * symbols;

    bool hasError;
} State;


void assembleInput(const char * baseName);

#endif /* __ASSEMBLER_H__ */
