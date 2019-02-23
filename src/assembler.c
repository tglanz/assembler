#include "assembler.h"

void firstPass(AssemblyState * state, FILE * file);
void secondPass(AssemblyState * state, FILE * file);
void generateOutputs(AssemblyState * state, const char * baseName);

void resetState(AssemblyState * state);
void updateDataSymbols(AssemblyState * state);

void handleExternDirective(AssemblyState * state, int lineNumber, const char * line){
    char arguments[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];
    int argIndex = 0;

    if (!tryGetDirectiveArgs(arguments, line) || strlen(arguments) == 0){
        logError("line %3d: missing arguments for extern directive", lineNumber);
        state->hasError = true;
        return;
    }
    
    while (getSplitComponent(argument, arguments, ' ', argIndex)){
        if (!symbolsSetInsert(state->symbols, SYMBOL_TYPE_EXTERN, argument, 0)){
            logWarning("line %3d: already have symbol: %s", lineNumber, argument);
        }
        ++argIndex;
    }

    if (argIndex == 0){
        logError("line %3d: expected at least one argument for extern directive",
                 lineNumber);
        state->hasError = true;
    }
}

void handleEntryDirective(AssemblyState * state, int lineNumber, const char * line){
    char arguments[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];
    int argIndex = 0;

    if (!tryGetDirectiveArgs(arguments, line) || strlen(arguments) == 0){
        logError("line %3d: missing arguments for entry directive", lineNumber);
        state->hasError = true;
        return;
    }

    while (getSplitComponent(argument, arguments, ' ', argIndex++)){
        if (argIndex > 0){
            logWarning("line %3d: entry directive can have only a single label as argument, ignoring `%s`", lineNumber, argument);
        } else if (!symbolsSetInsert(state->symbols, SYMBOL_TYPE_ENTRY, argument, 0)){
            logWarning("line %3d: already have an extern symbol: %s", lineNumber, argument);
        }
    }
}

void handleStringDirective(AssemblyState * state, int lineNumber, const char * line){
    int cursor;
    Word word;
    char stringData[MAX_LINE_LENGTH];

    if (getSplitComponent(stringData, line, '"', 2)){
        logError("line %3d: invalid string directive, expected only one argument", lineNumber);
        state->hasError = true;
    } else if (!getSplitComponent(stringData, line, '"', 1)){
        logError("line %3d: invalid string directive, expected argument", lineNumber);
        state->hasError = true;
    } else {
        cursor = 0;
        while (stringData[cursor] != '\0'){
            word.raw = stringData[cursor];
            wordsVectorAppend(state->data, word);
            state->DC += 1;
            ++cursor;
        }
    }
}

void handleDataDirective(AssemblyState * state, int lineNumber, const char * line){
    char arguments[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];
    int argIndex = 0;
    int integer;
    Word word;

    if (!tryGetDirectiveArgs(arguments, line) || strlen(arguments) == 0){
        logWarning("line %3d: no arguments for data directive, skipping", lineNumber);
        return;
    }

    while (getSplitComponent(argument, arguments, ',', argIndex++)){
        if (sscanf(argument, "%d", &integer) != 1){
            logError("line %3d: cannot parse argument `%s` to integer", lineNumber, argument);
            state->hasError = true;
            return;
        }

        word.raw = integer;
        wordsVectorAppend(state->data, word);
        ++state->DC;
    }
}

void assembleInput(const char * baseName) {
    /* assembler state to be carried on through the process */
    AssemblyState state;
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

void firstPass(AssemblyState * state, SourceFile * sourceFile){
    /* Decleration */
    unsigned int lineNumber;
    char line[MAX_LINE_LENGTH];

    char operation[MAX_LINE_LENGTH];
    char operationArguments[MAX_LINE_LENGTH];
    char directive[MAX_LINE_LENGTH];

    bool hasLabel;
    char label[MAX_LINE_LENGTH];

    /* Defaults */
    lineNumber = 0;

    while (readLine(sourceFile, line)){
        /* remove the newline at the end */
        ++lineNumber;

        logDebug("read line: line %3d: %s", lineNumber, line);

        if (!isMeaningfulLine(line)){
            logDebug("skipping, irrelevant line: %3d", lineNumber);
            continue;
        }

        hasLabel = tryGetLabel(label, line);
        if (hasLabel){
            logDebug("label assumed: %s", label);
            if (!isValidLabel(label)){
                logError("line %3d: invalid label, got: `%s`", lineNumber, label);
                state->hasError = true;
                continue;
            }
        }

        if (tryGetDirective(directive, line)){
            logDebug("directive assumed: %s", directive);
            switch (directiveTypeFromString(directive)){
                case DIRECTIVE_TYPE_ENTRY:
                    if (hasLabel) {
                        logWarning("line %3d: entry directive should not have a label. got label `%s`", lineNumber, label);
                    }
                    handleEntryDirective(state, lineNumber, line);
                    break;
                case DIRECTIVE_TYPE_EXTERN:
                    handleExternDirective(state, lineNumber, line);
                    break;
                case DIRECTIVE_TYPE_STRING:
                    if (hasLabel)
                        symbolsSetInsert(state->symbols, SYMBOL_TYPE_DATA, label, state->DC);
                    handleStringDirective(state, lineNumber, line);
                    break;
                case DIRECTIVE_TYPE_DATA:
                    if (hasLabel)
                        symbolsSetInsert(state->symbols, SYMBOL_TYPE_DATA, label, state->DC);
                    handleDataDirective(state, lineNumber, line);
                    break;
                case DIRECTIVE_TYPE_INVALID:
                    logError("line %3d: invalid directive: `%s`", lineNumber, directive);
                    state->hasError = true;
                    break;
            }
        } else if (tryGetOperation(operation, operationArguments, line, hasLabel)){
            logDebug("line %3d: got operation: %s", lineNumber, operation);
        } else {
            logError("line %3d: unknown error, unable to parse", lineNumber);
            state->hasError = true;
        }
    }

    if (!state->hasError){
        updateDataSymbols(state);
    }
}

void secondPass(AssemblyState * state, FILE * file){
    logError("secondPass::NotImplemented");
}

void generateOutputs(AssemblyState * state, const char * baseName){
    logError("generateOutputs::NotImplemented");
}

void updateDataSymbols(AssemblyState * state){
    logError("updateDataSymbols::NotImplemented");
}

void resetState(AssemblyState * state) {
    logDebug("Resetting state");
    state->IC = 0;
    state->DC = 0;
    state->hasError = false;
    state->symbols = symbolsSetNew();
    state->data = wordsVectorNew();
    state->instructions = wordsVectorNew();
}