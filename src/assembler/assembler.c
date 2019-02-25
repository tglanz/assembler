#include "assembler/assembler.h"

void generateOutputs(AssemblyState * state, const char * baseName);

bool assembleInput(const char * baseName) {
    SourceFile * sourceFile;
    AssemblyState * state;
    bool success = false;

    sourceFile = openSourceFile(baseName);
    if (sourceFile == NULL){
        logError("Could not open source file %s. Aborting", baseName);
        return false;
    }

    state = assemblyStateNew(MAX_LINE_LENGTH);

    runFirstPass(state, sourceFile);
    if (!state->hasError){
        /* rewind file */
        fseek(sourceFile, 0, SEEK_SET);
        runSecondPass(state, sourceFile);
    }

    fclose(sourceFile);
    if (!state->hasError){
        generateOutputs(state, baseName);
    }

    success = !state->hasError;
    assemblyStateFree(state);

    return success;
}


void generateOutputs(AssemblyState * state, const char * baseName){
    state->hasError = true;
    logError("generateOutputs::NotImplemented");
}

