#include "assembler.h"

void firstPass(State * state, FILE * file);
void secondPass(State * state, FILE * file);
void generateOutputs(State * state, const char * baseName);

void resetState(State * state);
void updateDataSymbols(State * state);
void createFileName(char * destination, const char * baseName, const char * extension);

void appendExternSymbols(SymbolsVector * symbols, int lineNumber, const char * line, const char * directive){
    char label[MAX_LINE_LENGTH];
    char tmp[MAX_LINE_LENGTH];
    int idx = 1;

    tryGetSplitComponent(line, '.', 1, tmp);
    while (tryGetSplitComponent(tmp, ' ', idx, label)){
        if (!symbolsVectorAdd(symbols, SYMBOL_TYPE_NONE, withoutWhitespace(label), 0)){
            logWarning("line %3d: already have an extern symbol: %s", lineNumber, label);
        }
        ++idx;
    }

    if (idx == 1){
        logWarning("line %3d: at least one label should be provided with an extern directive", lineNumber);
    }
}

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

    /* Defaults */
    lineNumber = 0;

    while (readLine(file, line)){
        /* remove the newline at the end */
        ++lineNumber;
        
        if (isEmptyLine(line) || isCommentLine(line)){
            continue;
        }

        hasLabel = tryGetLabel(line, label);
        hasDirective = tryGetDirective(line, directive);

        if (hasDirective){
            switch (directiveTypeFromString(directive)){
                
                case DIRECTIVE_TYPE_ENTRY:
                    if (hasLabel){
                        logWarning("line %3d: entry directive should not have a label. got label `%s`",
                            lineNumber, label);
                    }
                    break;

                case DIRECTIVE_TYPE_EXTERN:
                    appendExternSymbols(state->symbols, lineNumber, line, directive);
                    break;
                    
                case DIRECTIVE_TYPE_DATA:
                case DIRECTIVE_TYPE_STRING:
                    logWarning("line %3d: Not implemented directive: %s", lineNumber, directive);
                case DIRECTIVE_TYPE_INVALID:
                    state->hasError = true;
                    break;
            }
        } else {

        }
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
    state->symbols = symbolsVectorNew();
}

void createFileName(char * destination, const char * baseName, const char * extension) {
    strcpy(destination, baseName);
    strcat(destination, ".");
    strcat(destination, extension);
}
