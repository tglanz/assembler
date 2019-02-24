#include "assembler/assemblyState.h"

AssemblyState createAssemblyState() {
    AssemblyState state;
    state.IC = 0;
    state.DC = 0;
    state.hasError = false;
    state.symbols = symbolsSetNew();
    state.data = wordsVectorNew("data");
    state.instructions = wordsVectorNew("instructions");
    return state;
}