#ifndef __ASSEMBLY_STATE_H__
#define __ASSEMBLY_STATE_H__

#include "core.h"
#include "log.h"
#include "words.h"
#include "symbols.h"

typedef struct {
    unsigned int IC;
    unsigned int DC;

    SymbolsSet * symbols;
    WordsVector * data;
    WordsVector * instructions;

    int lineNumber;
    char * line;

    bool hasError;
} AssemblyState;

AssemblyState * assemblyStateNew(int maxLineLength);
void assemblyStateFree(AssemblyState * state);

#endif /* __ASSEMBLY_STATE_H__ */