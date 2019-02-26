#include "assembler/assembler.h"

void generateOutputs(AssemblyState * state, string baseName);

bool assembleInput(string baseName) {
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


void generateOutputs(AssemblyState * state, string baseName){
    generateEntriesFile(baseName, state);
    generateExternalsFile(baseName, state);
    generateObjectFile(baseName, state);
}

