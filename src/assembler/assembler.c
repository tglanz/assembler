#include "assembler/assembler.h"

void generateOutputs(AssemblyState * state, string baseName);

bool assembleInput(string baseName) {
    SourceFile * sourceFile;
    AssemblyState * state;
    bool success = false;

    /* open file */
    sourceFile = openSourceFile(baseName);
    if (sourceFile == NULL){
        logError("Could not open source file %s. Aborting", baseName);
        return false;
    }

    /* initialize state */
    state = assemblyStateNew(MAX_LINE_LENGTH);

    /* run the passes */
    runFirstPass(state, sourceFile);
    if (!state->hasError){
        /* rewind file */
        fseek(sourceFile, 0, SEEK_SET);
        runSecondPass(state, sourceFile);
    }

    /* cleanup */
    fclose(sourceFile);
    if (!state->hasError){
        /* only if there are no errors we want to generate outputs */
        generateOutputs(state, baseName);
    }

    success = !state->hasError;
    assemblyStateFree(state);

    return success;
}


void generateOutputs(AssemblyState * state, string baseName){
    /* go through all outputs and generate them */
    generateEntriesFile(baseName, state);
    generateExternalsFile(baseName, state);
    generateObjectFile(baseName, state);
}

