#include "assembler/assembler.h"

void generateOutputs(AssemblyState * state, const char * baseName);

bool assembleInput(const char * baseName) {
    SourceFile * sourceFile;
    AssemblyState state = createAssemblyState();

    sourceFile = openSourceFile(baseName);
    if (sourceFile == NULL){
        logError("Could not open source file %s. Aborting", baseName);
        return false;
    }

    runFirstPass(&state, sourceFile);
    if (!state.hasError){
        /* rewind file */
        fseek(sourceFile, 0, SEEK_SET);
        runSecondPass(&state, sourceFile);
    }

    fclose(sourceFile);
    if (!state.hasError){
        generateOutputs(&state, baseName);
    }

    return !state.hasError;
}


void generateOutputs(AssemblyState * state, const char * baseName){
    state->hasError = true;
    logError("generateOutputs::NotImplemented");
}

