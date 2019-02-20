#include "assembler.h"

void firstPass(State * state, FILE * file);
void secondPass(State * state, FILE * file);
void generateOutputs(State * state, const char * baseName);

void resetState(State * state);
void updateDataSymbols(State * state);
void createFileName(char * destination, const char * baseName, const char * extension);
DirectiveType directiveTypeFromString(const char * string);

void assembleInput(const char * baseName) {
    /* assembler state to be carried on through the process */
    State state;
    FILE * sourceFile;
    char filePath[MAX_FILE_PATH_LENGTH];

    /* initialize to a known state, determined elsewhere */
    resetState(&state);

    logInfo("Processing: %s", baseName);

    createFileName(filePath, baseName, "as");
    logDebug("File name assumed to be: %s", filePath);

    sourceFile = fopen(filePath, "r");
    if (sourceFile == NULL){
        logError("Could not open file: %s. Aborting", filePath);
        return;
    }

    firstPass(&state, sourceFile);
    if (!state.hasError){
        /* rewind file */
        fseek(sourceFile, 0, SEEK_SET);
        secondPass(&state, sourceFile);
    }

    fclose(sourceFile);
    if (!state.hasError){
        generateOutputs(&state, baseName);
    }
}

void firstPass(State * state, FILE * file){
    /* Decleration */
    unsigned int lineNumber;

    char line[MAX_LINE_LENGTH];
    char label[MAX_LINE_LENGTH];
    char directive[MAX_LINE_LENGTH];

    bool hasLabel;
    bool hasDirective;

    DirectiveType directiveType;

    /* Defaults */
    lineNumber = 1;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL){
        /* remove the newline at the end */
        line[strlen(line) - 1] = '\0';

        hasLabel = tryGetLabel(line, label);
        hasDirective = tryGetDirective(line, directive);

        if (hasDirective){
            switch (directiveType){
                
            }
            directiveType = directiveTypeFromString(directive);
            if (directiveType == DIRECTIVE_TYPE_DATA || directiveType == DIRECTIVE_TYPE_STRING){
                // add to symbols,
                // push arguments to memory
            } else if (directiveType == DIRECTIVE_TYPE_ENTRY || directiveType == DIRECTIVE_TYPE_EXTERN){
                // add to symbols
            }
        }

        ++lineNumber;
    }

    if (!state->hasError){
        updateDataSymbols(state);
    }
}

void secondPass(State * state, FILE * file){
    logError("secondPass::NotImplemented");
}

void generateOutputs(State * state, const char * baseName){
    logError("generateOutputs::NotImplemented");
}

void updateDataSymbols(State * state){
    logError("updateDataSymbols::NotImplemented");
}

void resetState(State * state) {
    logDebug("Resetting state");
    state->IC = 0;
    state->DC = 0;
    state->hasError = false;
}

void createFileName(char * destination, const char * baseName, const char * extension) {
    strcpy(destination, baseName);
    strcat(destination, ".");
    strcat(destination, extension);
}

DirectiveType directiveTypeFromString(const char * string){
    if (strcmp(string, "data") == 0){
        return DIRECTIVE_TYPE_DATA;
    } else if (strcmp(string, "string") == 0){
        return DIRECTIVE_TYPE_STRING;
    } else if (strcmp(string, "entry") == 0){
        return DIRECTIVE_TYPE_ENTRY;
    } else if (strcmp(string, "extern") == 0){
        return DIRECTIVE_TYPE_EXTERN;
    } else {
        return DIRECTIVE_TYPE_INVALID;
    }
}