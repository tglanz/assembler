#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "log.h"
#include "bitflags.h"

#define SYMBOLS_TABLE_INITIAL_CAPACITY (4)
#define SYMBOLS_TABLE_GROW_FACTOR (2)

typedef enum {
    SYMBOL_TYPE_NONE,
    SYMBOL_TYPE_DATA,
    SYMBOL_TYPE_CODE
} SymbolType;

typedef enum {
    SYMBOL_FLAG_NONE = 0x0,
    SYMBOL_FLAG_ENTRY = 0x1,
    SYMBOL_FLAG_EXTERNAL = 0x2
} SymbolFlags;

typedef struct {
    char * key;
    SymbolType type;
    SymbolFlags flags;
    uint value;
} Symbol;

typedef struct {
    string name;
    uint size;
    uint capacity;
    Symbol * data;
} SymbolsTable;

SymbolsTable * symbolsTableNew(string name);
void symbolsTableFree(SymbolsTable * table);
bool symbolsTableInsert(SymbolsTable * table, SymbolType type, string key, uint value);
void symbolsTableAppend(SymbolsTable * table, SymbolType type, string key, uint value);
Symbol * symbolsTableGet(SymbolsTable * table, uint idx);
Symbol * symbolsTableFind(SymbolsTable * table, string key);
bool symbolsTableFlag(SymbolsTable * table, string key, uint flag);

#endif /* __SYMBOLS_H__ */