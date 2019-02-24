#include "symbols.h"

/* internal utility, re-allocate memory if required */
void symbolsSetgrowAccordingly(SymbolsSet * set){
    int idx;
    Symbol * data;

    if (set != NULL){
        if (set->size == set->capacity){
            if (set->capacity == 0){
                set->capacity = SYMBOLS_SET_INITIAL_CAPACITY;
            } else {
                set->capacity *= SYMBOLS_SET_GROW_FACTOR;
            }
            data = (Symbol*)calloc(set->capacity, sizeof(Symbol));
            if (set->data != NULL){
                for (idx = 0; idx < set->size; ++idx){
                    data[idx] = set->data[idx];
                }
                free(set->data);
            }
            set->data = data;
            logDebug("symbols set has regrown from %d to %d", set->size, set->capacity);
        }
    }
}

SymbolsSet * symbolsSetNew() {
    SymbolsSet * set = (SymbolsSet*)malloc(sizeof(SymbolsSet));;
    set->capacity = 0;
    set->size = 0;
    set->data = NULL;
    symbolsSetgrowAccordingly(set);
    return set;
}
void symbolsSetFree(SymbolsSet * set) {
    int index;

    if (set != NULL){
        if (set->data != NULL){
            for (index = 0; index < set->size; ++index){
                free(set->data[index].key);
            }

            free(set->data);
        }
    }

    free(set);
}
bool symbolsSetInsert(SymbolsSet * set, SymbolType type, const char * key, unsigned int value) {
    Symbol symbol;

    if (symbolsSetFind(set, key) != NULL){
        return false;
    }

    symbol.type = type;
    symbol.value = value;
    symbol.key = strdup(key);

    set->data[set->size] = symbol;
    ++set->size;
    logDebug("added symbol: %s, value: %d, type: %d", symbol.key, symbol.value, symbol.type);
    symbolsSetgrowAccordingly(set);
    return true;
}

const Symbol * symbolsSetFind(SymbolsSet * set, const char * key){
    int idx;
    for (idx = 0; idx < set->size; ++idx){
        if (strcmp(set->data[idx].key, key) == 0){
            return &set->data[idx];
        }
    }    
    return NULL;
}