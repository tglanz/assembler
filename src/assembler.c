#include "assembler.h"

void assemblerResetState(State * state) {
    logDebug("Resetting state");
    state->IC = 0;
    state->DC = 0;
    state->firstPassSuccess = false;
    state->secondPassSuccess = false;
}

void assemblerLogState(const State * state){
    logDebug("State(IC=%d, DC=%d)", state->IC, state->DC);
}

void assemblerProcessInput(const char * baseName) {
    /* assembler state to be carried on through the process */
    State state;
    FILE * sourceFile;
    char filePath[MAX_FILE_PATH_LENGTH];

    /* initialize to a known state, determined elsewhere */
    assemblerResetState(&state);

    logInfo("Processing: %s", baseName);

    assemblerFileName(filePath, baseName, "as");
    logDebug("File name assumed to be: %s", filePath);

    sourceFile = fopen(filePath, "r");
    if (sourceFile == NULL){
        logError("Could not open file: %s. Aborting", filePath);
        return;
    }

    assemblerRunFirstPass(&state, sourceFile);
    if (state.firstPassSuccess){
        /* rewind file */
        fseek(sourceFile, 0, SEEK_SET);
        assemblerRunSecondPass(&state, sourceFile);
    }

    fclose(sourceFile);
    if (state.secondPassSuccess){
        assemblerGenerateOutputs(&state, baseName);
    }
}


void assemblerRunFirstPass(State * state, FILE * file) {
    unsigned int lineNumber;
    char line[MAX_LINE_LENGTH + 1];
    StatementType statementType;

    lineNumber = 1;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL){
        /* remove the newline at the end */
        line[strlen(line) - 1] = '\0';

        logDebug("parsing line%5d: %s", lineNumber, line);
        switch (statementType = parserDetermineStatementType(line)){
            case STATEMENT_TYPE_COMMENT:
            case STATEMENT_TYPE_EMPTY:
                break;
            case STATEMENT_TYPE_DIRECTIVE:
            case STATEMENT_TYPE_INSTRUCTION:
                logWarning("did not implement logic for statement type line %5d: %s", lineNumber, line);
                break;
            case STATEMENT_TYPE_UNKNOWN:
                logError("unable to determine statement type for line %5d: %s", lineNumber, line);
        }
        ++lineNumber;
    }

    logWarning("assemblerRunFirstPass::NotImplemented");
}

void assemblerRunSecondPass(State * state, FILE * file) {
    logError("assemblerRunSecondPass::NotImplemented");
}

void assemblerGenerateOutputs(State * state, const char * baseName) {
    logError("assemblerGenerateOutputs::NotImplemented");
}

void assemblerFileName(char * destination, const char * baseName, const char * extension) {
    strcpy(destination, baseName);
    strcat(destination, ".");
    strcat(destination, extension);
}