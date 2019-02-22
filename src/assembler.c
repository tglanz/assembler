#include "assembler.h"

void firstPass(State * state, FILE * file);
void secondPass(State * state, FILE * file);
void generateOutputs(State * state, const char * baseName);

void resetState(State * state);
void updateDataSymbols(State * state);

void handleExternDirective(SymbolsVector * symbols, int lineNumber, const char * line, const char * directive){
    char arguments[MAX_LINE_LENGTH];
    char label[MAX_LINE_LENGTH];
    int idx = 1;

    getSplitComponent(arguments, line, '.', 1);
    while (getSplitComponent(label, arguments, ' ', idx)){
        trimStart(label, label);
        if (!symbolsVectorAdd(symbols, SYMBOL_TYPE_NONE, label, 0)){
            logWarning("line %3d: already have an extern symbol: %s", lineNumber, label);
        }
        ++idx;
    }

    if (idx == 1){
        logWarning("line %3d: at least one label should be provided with an extern directive", lineNumber);
    }
}

int handleDataDirective(SymbolsVector * symbols, int lineNumber, const char * line, const char * directive){
    logWarning("handleDataDirective::TODO");
    /*
        get string argument -> array of words
        put the array of words in the memory (not existant yet)
        return how many words have been encoded into the memort
    */
}

int handleStringDirective(SymbolsVector * symbols, int lineNumber, const char * line, const char * directive){
    logWarning("handleDataDirective::TODO");
    /*
        get int arguments -> array of words
        put the array of words in the memory (not existant yet)
        return how many words have been encoded into the memort
    */
}

void assembleInput(const char * baseName) {
    /* assembler state to be carried on through the process */
    State state;
    SourceFile * sourceFile;

    /* initialize to a known state, determined elsewhere */
    resetState(&state);

    logInfo("Processing: %s", baseName);

    sourceFile = openSourceFile(baseName);
    if (sourceFile == NULL){
        logError("Could not open source file %s. Aborting", baseName);
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

void firstPass(State * state, SourceFile * sourceFile){
    /* Decleration */
    unsigned int lineNumber;
    char line[MAX_LINE_LENGTH];

    char directive[MAX_LINE_LENGTH];

    bool hasLabel;
    char label[MAX_LINE_LENGTH];

    /* Defaults */
    lineNumber = 0;

    while (readLine(sourceFile, line)){
        /* remove the newline at the end */
        ++lineNumber;

        if (!isMeaningfulLine(line)){
            logDebug("irrelevant line: %3d", lineNumber);
            continue;
        }

        hasLabel = tryGetLabel(label, line);
        if (hasLabel && !isValidLabel(label)){
            logError("line %3d: invalid label, got: `%s`", lineNumber, label);
            state->hasError = true;
            continue;
        }

        if (tryGetDirective(directive, line)){
            switch (directiveTypeFromString(directive)){
                
                case DIRECTIVE_TYPE_ENTRY:
                    if (hasLabel){
                        logWarning("line %3d: entry directive should not have a label. got label `%s`",
                            lineNumber, label);
                    }
                    break;

                case DIRECTIVE_TYPE_EXTERN:
                    handleExternDirective(state->symbols, lineNumber, line, directive);
                    break;
                    
                case DIRECTIVE_TYPE_DATA:
                case DIRECTIVE_TYPE_STRING:
                    if (hasLabel){
                        symbolsVectorAdd(state->symbols, SYMBOL_TYPE_DATA, label, state->DC);
                    }
                    TODO:: handleDataDirective / handleStringDirective
                case DIRECTIVE_TYPE_INVALID:
                    logError("line %3d: invalid directive: `%s`", directive);
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