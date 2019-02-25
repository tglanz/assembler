#include "assembler/assemblyState.h"

AssemblyState * assemblyStateNew(int maxLineLength){
    AssemblyState * state = (AssemblyState*)malloc(sizeof(AssemblyState));
    state->IC = 0;
    state->DC = 0;
    state->hasError = false;
    state->symbols = symbolsSetNew();
    state->data = wordsVectorNew("data");
    state->instructions = wordsVectorNew("instructions");
    state->lineNumber = 0;
    state->line = (char*)calloc(maxLineLength, sizeof(char));
    return state;
}

void assemblyStateFree(AssemblyState * state){
    symbolsSetFree(state->symbols);
    wordsVectorFree(state->data);
    wordsVectorFree(state->instructions);
    free(state->line);
}