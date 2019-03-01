/**
 * Symbols related definitions.
 * 
 * Define the symbols table, with unique/non-unique insertions
 * 
 * In this context - 
 *      - capacity will mean allocated space count (1 unit for a single value)
 *      - size will mean the used up space count (1 unit for a single value)
 */
#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

/* standard */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* servics */
#include "core.h"
#include "log.h"
#include "bitflags.h"

/* The capacity of newly created SymbolsTable */
#define SYMBOLS_TABLE_INITIAL_CAPACITY (4)

/* How much to grow and re-allocate the memory when reaching end of capacity */
#define SYMBOLS_TABLE_GROW_FACTOR (2)

/* The available types of symbols */
typedef enum {
    /* Default, non specific */
    SYMBOL_TYPE_NONE,
    /* References to the data section */
    SYMBOL_TYPE_DATA,
    /* References to the code section */
    SYMBOL_TYPE_CODE
} SymbolType;

/* Additional flags that can be attached to a symbol */
typedef enum {
    /* No flag */
    SYMBOL_FLAG_NONE = 0x0,
    /**
     * Indicates that the symbol is marked as an entry symbol
     * and should be appended to the entries output
     */
    SYMBOL_FLAG_ENTRY = 0x1,
    /**
     * Indicates that the symbol is marked as an external symbol
     * and should be appended to the externals output
     */
    SYMBOL_FLAG_EXTERNAL = 0x2
} SymbolFlags;

/**
 * Encapsulate relevant fields of a symbol
 */
typedef struct {
    /* A key of the symbol, will determine uniquness */
    char * key;
    /* the type of the symbol */
    SymbolType type;
    /* attached flags for the symbol */
    SymbolFlags flags;
    /* a value attached to the symbol, has meaning according to context (Types and flags) */
    uint value;
} Symbol;

/**
 * A data structure that will contain symbools and will internally manage
 * dynamic re-allocation
*/
typedef struct {
    /**
     * a name for the data structure, can be used for debugging, reporting etc..
     * no real functionality as of now
     */
    string name;

    /* the size of the container */
    uint size;

    /* the capacity of the container */
    uint capacity;

    /* the symbols held within this container */
    Symbol * data;
} SymbolsTable;

/**
 * Create a new symbols table
 * 
 * Arguments
 *      name: string - the name to assign to the container
 * 
 * Return
 *      SymbolsTable* - newly created container
 */
SymbolsTable * symbolsTableNew(string name);

/**
 * Free the given container and all of it's resources
 * 
 * Arguments
 *      table: SymbolsTable* - the container to free
 */
void symbolsTableFree(SymbolsTable * table);

/**
 * Inserts a symbol to the container.
 * If a symbol with the same key exists, it will not be inserted.
 * 
 * Arguments
 *      table: SymbolsTable* - the container to insert into
 *      type: SymbolType - the type of the symbol to insert
 *      key: string - the key of the symbol to insert
 *      value: uint - the value of the symbol to insert
 * 
 * Return
 *      bool - true only if the symbol was inserted
 *          false if the key already existed
 */
bool symbolsTableInsert(SymbolsTable * table, SymbolType type, string key, uint value);

/**
 * Appends a symbol to the container, without any checks.
 * 
 * Arguments
 *      table: SymbolsTable* - the container to append to
 *      type: SymbolType - the type of the symbol to append
 *      key: string - the key of the symbol to append
 *      value: uint - the value of the symbol to append
 */
void symbolsTableAppend(SymbolsTable * table, SymbolType type, string key, uint value);

/**
 * Retrievs a symbol from the given container, according to it's index
 * 
 * Arguments
 *      table: SymbolsTable* - the container to get symbol from
 *      idx: uint - the index of the symbol to get
 * 
 * Returns
 *      Symbol* - a mutable reference to a symbol
 *          NULL if no symbol in the given index
 */
Symbol * symbolsTableGet(SymbolsTable * table, uint idx);

/**
 * Retrievs a symbol from the given container, according to it's key
 * 
 * Note, that the returned symbol is the first encountered with the given key.
 * 
 * Arguments
 *      table: SymbolsTable* - the container to get symbol from
 *      key: string - the key of the symbol to get
 * 
 * Returns
 *      Symbol* - a mutable reference to a symbol
 *          NULL if no symbol in the given index
 */
Symbol * symbolsTableFind(SymbolsTable * table, string key);

/**
 * Sets a flag to a symbol with the same key
 * 
 * Arguments
 *      table: SymbolsTable* - the container the symbol is in
 *      key: string - the key of the symbol
 *      flag: uint - the flag to set
 * 
 * Returns
 *      bool - true if the symbol was found, and the flag was set
 */
bool symbolsTableFlag(SymbolsTable * table, string key, uint flag);

#endif /* __SYMBOLS_H__ */