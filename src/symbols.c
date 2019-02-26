#include "symbols.h"

/* internal utility, re-allocate memory if required */
void symbolsTableGrowAccordingly(SymbolsTable * table){
    uint idx;
    Symbol * data;

    if (table != NULL){
        if (table->size == table->capacity){
            if (table->capacity == 0){
                table->capacity = SYMBOLS_TABLE_INITIAL_CAPACITY;
            } else {
                table->capacity *= SYMBOLS_TABLE_GROW_FACTOR;
            }
            data = (Symbol*)calloc(table->capacity, sizeof(Symbol));
            if (table->data != NULL){
                for (idx = 0; idx < table->size; ++idx){
                    data[idx] = table->data[idx];
                }
                free(table->data);
            }
            table->data = data;
            logDebug("symbols table has regrown from %d to %d", table->size, table->capacity);
        }
    }
}

SymbolsTable * symbolsTableNew(string name) {
    SymbolsTable * table = (SymbolsTable*)malloc(sizeof(SymbolsTable));;
    table->name = name;
    table->capacity = 0;
    table->size = 0;
    table->data = NULL;
    symbolsTableGrowAccordingly(table);
    return table;
}
void symbolsTableFree(SymbolsTable * table) {
    uint index;

    if (table != NULL){
        if (table->data != NULL){
            for (index = 0; index < table->size; ++index){
                free(table->data[index].key);
            }

            free(table->data);
        }
    }

    free(table);
}

void symbolsTableAppend(SymbolsTable * table, SymbolType type, string key, uint value) {
    /* can have duplicates by key */
    Symbol symbol;
    symbol.type = type;
    symbol.value = value;
    symbol.flags = SYMBOL_FLAG_NONE;
    symbol.key = strdup(key);
    table->data[table->size] = symbol;
    ++table->size;
    logDebug("appended symbol: %s, value: %d, type: %d", symbol.key, symbol.value, symbol.type);
    symbolsTableGrowAccordingly(table);
}

bool symbolsTableInsert(SymbolsTable * table, SymbolType type, string key, uint value) {
    /* cannot have duplicates by key */
    if (symbolsTableFind(table, key) != NULL){
        return false;
    }

    symbolsTableAppend(table, type, key, value);
    return true;
}

Symbol * symbolsTableGet(SymbolsTable * table, uint idx) {
    if (idx < table->size){
        return &table->data[idx];
    }
    return NULL;
}

Symbol * symbolsTableFind(SymbolsTable * table, string key){
    uint idx;
    for (idx = 0; idx < table->size; ++idx){
        if (strcmp(table->data[idx].key, key) == 0){
            return &table->data[idx];
        }
    }    
    return NULL;
}

bool symbolsTableFlag(SymbolsTable * table, string key, uint flag) {
    Symbol * symbol = symbolsTableFind(table, key);
    if (symbol != NULL){
        set_flag(flag, &symbol->flags);
        return true;
    }
    return false;
}