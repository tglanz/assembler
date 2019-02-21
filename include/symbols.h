#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "log.h"

#define SYMBOLS_DEFAULT_CAPACITY (4)

typedef enum {
    SYMBOL_TYPE_DATA,
    SYMBOL_TYPE_CODE,
    SYMBOL_TYPE_NONE
} SymbolType;

typedef struct {
    char * label;
    SymbolType type;
    unsigned int value;
} Symbol;

typedef struct {
    unsigned int size;
    unsigned int capacity;
    Symbol * data;
} SymbolsVector;

SymbolsVector * symbolsVectorNew();
void symbolsVectorFree(SymbolsVector * symbols);
bool symbolsVectorAdd(SymbolsVector * symbols, SymbolType type, const char * label, unsigned int value);

#endif /* __SYMBOLS_H__ */