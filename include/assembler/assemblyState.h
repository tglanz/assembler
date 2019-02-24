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

    bool hasError;
} AssemblyState;

#endif /* __ASSEMBLY_STATE_H__ */