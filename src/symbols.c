#include "symbols.h"

void growAccordingly(SymbolsVector * symbols){
    int idx;
    Symbol * data;

    if (symbols == NULL){
        return;
    }

    if (symbols->size == symbols->capacity){
        symbols->capacity *= 2;
        data = (Symbol*)calloc(symbols->capacity, sizeof(Symbol));
        if (symbols->data != NULL){
            for (idx = 0; idx < symbols->size; ++idx){
                data[idx] = symbols->data[idx];
            }
        }
        free(symbols->data);
        symbols->data = data;
    }
}

SymbolsVector * symbolsVectorNew() {
    SymbolsVector * symbols = (SymbolsVector*)malloc(sizeof(SymbolsVector));;
    symbols->capacity = 0;
    symbols->capacity = 0;
    symbols->size = 0;
    growAccordingly(symbols);
    return symbols;
}
void symbolsVectorFree(SymbolsVector * symbols) {
    int index;

    if (symbols == NULL){
        return;
    }

    if (symbols->data != NULL){

        for (index = 0; index < symbols->size; ++index){
            free(symbols->data[index].label);
        }

        free(symbols->data);
    }

    free(symbols);
}
bool symbolsVectorAdd(SymbolsVector * symbols, SymbolType type, const char * label, unsigned int value) {
    int idx;
    Symbol symbol;

    for (idx = 0; idx < symbols->size; ++idx){
        if (strcmp(symbols->data[idx].label, label) == 0){
            return false;
        }
    }

    symbol.type = type;
    symbol.value = value;
    symbol.label = strdup(label);

    symbols->data[symbols->size] = symbol;
    ++symbols->size;
    return true;
}