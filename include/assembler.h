#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <ctype.h>
#include <string.h>

#include "core.h"
#include "log.h"
#include "sourceFile.h"
#include "parsing.h"
#include "symbols.h"
#include "words.h"
#include "instructions.h"

typedef struct {
    unsigned int IC;
    unsigned int DC;

    SymbolsSet * symbols;
    WordsVector * data;
    WordsVector * instructions;

    bool hasError;
} AssemblyState;

void assembleInput(const char * baseName);

#endif /* __ASSEMBLER_H__ */
