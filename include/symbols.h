#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "log.h"

#define SYMBOLS_SET_INITIAL_CAPACITY (4)
#define SYMBOLS_SET_GROW_FACTOR (2)

typedef enum {
    SYMBOL_TYPE_DATA,
    SYMBOL_TYPE_CODE,
    SYMBOL_TYPE_ENTRY, 
    SYMBOL_TYPE_EXTERN
} SymbolType;

typedef struct {
    char * key;
    SymbolType type;
    unsigned int value;
} Symbol;

typedef struct {
    unsigned int size;
    unsigned int capacity;
    Symbol * data;
} SymbolsSet;

SymbolsSet * symbolsSetNew();
void symbolsSetFree(SymbolsSet * set);
bool symbolsSetInsert(SymbolsSet * set, SymbolType type, const char * key, unsigned int value);
const Symbol * symbolsSetFind(SymbolsSet * set, const char * key);

#endif /* __SYMBOLS_H__ */