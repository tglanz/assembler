/**
 * Defines the AssemblyState.
 * 
 * In order to avoid passing too many pointers, or using a global state,
 * define the AssemblyState which encapsulate all of the mutatable values that
 * the assembler parts need to to their job.
 */
#ifndef __ASSEMBLY_STATE_H__
#define __ASSEMBLY_STATE_H__

#include "core.h"
#include "log.h"
#include "words.h"
#include "symbols.h"

typedef struct {
    /* keep track on how many instructions have been encoded */
    uint IC;

    /* keep track on how many words have been encoded to data section */
    uint DC;

    /* the symbols table, keep track on encountered symbols and their positions */
    SymbolsTable * symbols;

    /**
     * A specific symbols table, for externalized symbols.
     * Those symbols can appear more than once, so are kept seperately
     */
    SymbolsTable * externalSymbols;

    /* Data section - Encoded data words */
    WordsVector * data;

    /* Code section - Encoded instructions words */
    WordsVector * code;

    /* the current line number being processed */
    int lineNumber;

    /* the current line being processed */
    char * line;

    /* a flag that indicates that the assembler had encountered an unrecoverable error */
    bool hasError;

} AssemblyState;

/**
 * Creates a new assembly state
 * 
 * Arguments
 *      maxLineLength: int - the maximum characters in a line of the source file
 */
AssemblyState * assemblyStateNew(int maxLineLength);

/**
 * Frees the given state's resources
 * 
 * Arguments
 *      state: AssemblyState* - a pointer to free the resources of
 */
void assemblyStateFree(AssemblyState * state);

#endif /* __ASSEMBLY_STATE_H__ */