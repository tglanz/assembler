#include "assembler/firstPass.h"

void handleExternDirective(AssemblyState * state, int lineNumber, const char * line){
    char arguments[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];
    int argIndex = 0;

    if (!tryGetDirectiveArgs(arguments, line) || strlen(arguments) == 0){
        logError("line %3d: missing arguments for extern directive", lineNumber);
        state->hasError = true;
        return;
    }
    
    while (getSplitComponent(argument, arguments, " ", argIndex)){
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

    while (getSplitComponent(argument, arguments, " ", argIndex)){
        if (argIndex > 0){
            logWarning("line %3d: entry directive can have only a single label as argument, ignoring `%s`",
                       lineNumber, argument);
        } else if (!symbolsSetInsert(state->symbols, SYMBOL_TYPE_ENTRY, argument, 0)){
            logWarning("line %3d: duplicate symbol: `%s`", lineNumber, argument);
        }
        ++argIndex;
    }
}

void handleStringDirective(AssemblyState * state, int lineNumber, const char * line){
    int cursor;
    Word word;
    char stringData[MAX_LINE_LENGTH];

    if (getSplitComponent(stringData, line, "\"", 2)){
        logError("line %3d: invalid string directive, expected only one argument", lineNumber);
        state->hasError = true;
    } else if (!getSplitComponent(stringData, line, "\"", 1)){
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

    while (getSplitComponent(argument, arguments, ",", argIndex++)){
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

void handleOperation(AssemblyState * state, int lineNumber, const char * operation, const char * arguments) {
    const InstructionModel * instructionModel;
    unsigned int idx, operandsCount, dataWordsCount;
    char argument[MAX_LINE_LENGTH];
    InstructionWord instructionWord;

    instructionModel = findInstructionModel(operation);
    if (instructionModel == NULL){
        logError("line %3d: unknown operation: `%s`", operation);
        state->hasError = true;
        return;
    }

    operandsCount = 0;
    if (strlen(arguments) != 0){
        operandsCount = countCharacterOccurrences(arguments, 0, ',') + 1;
    }
    if (operandsCount != getModelOperandsCount(instructionModel)){
        logError("line %3d: invalid argument count for operation `%s`. expected %d",
                 instructionModel->operation, operandsCount);
        state->hasError = true;
        return;
    }

    /* TODO: Refactor */
    instructionWord.word.raw = 0;
    for (idx = 0; idx < operandsCount; ++idx){

        getSplitComponent(argument, arguments, ",", idx);

        if (instructionModel->addressTypes[0] == ADDRESS_TYPE_NONE){
            ++idx;
        }

        if (idx == 0){
            instructionWord.fields.sourceAddressType = oeprandStringToAddressType(argument);
            if ((instructionWord.fields.sourceAddressType & instructionModel->addressTypes[0]) != 
                instructionWord.fields.sourceAddressType){
                    logError("line %3d: invalid address type for source operand: `%s`",
                             lineNumber, argument);
                    state->hasError = true;
                }
        } else if (idx == 1) {
            instructionWord.fields.destinationAddressType = oeprandStringToAddressType(argument);
            if ((instructionWord.fields.destinationAddressType & instructionModel->addressTypes[1]) != 
                instructionWord.fields.destinationAddressType){
                    logError("line %3d: invalid address type for destination operand: `%s`",
                             lineNumber, argument);
                    state->hasError = true;
                }
        } else {
            logError("line %3d: more than 2 operands is not supported, shuldn't reach this case");
            state->hasError = true;
        }
    }

    instructionWord.fields.opcode = instructionModel->code;
    wordsVectorAppend(state->instructions, instructionWord.word);

    dataWordsCount = getDataWordsCount(instructionWord.fields.sourceAddressType,
                                       instructionWord.fields.destinationAddressType);
    for (idx = 0; idx < dataWordsCount; ++idx){
        instructionWord.word.raw = 0;
        wordsVectorAppend(state->instructions, instructionWord.word);
    }

    state->IC += 1 + dataWordsCount;
}

void updateDataSymbolsWithInstructionsCounter(AssemblyState * state) {
    int idx;
    unsigned int newValue;

    Symbol * symbol;
    for (idx = 0; idx < state->symbols->size; ++idx){
        symbol = &state->symbols->data[idx];
        if (symbol->type == SYMBOL_TYPE_DATA){
            newValue = symbol->value + state->IC;
            logDebug("updating data sybmol: %s value %d, to %d", symbol->key, symbol->value, newValue);
            symbol->value = newValue;
        }
    }
}

void runFirstPass(AssemblyState * state, SourceFile * sourceFile){
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

        logDebug("first pass, read line: line %3d: %s", lineNumber, line);

        if (!isMeaningfulLine(line)){
            continue;
        }

        hasLabel = tryGetLabel(label, line);
        if (hasLabel && !isValidLabel(label)){
            
            if (strlen(label) > 0 && isWhitespaceCharacter(label[0])){
                logError("line %3d: invalid label, got: `%s`. hint: labels must not start with a whitespace!",
                         lineNumber, label);
            } else {
                logError("line %3d: invalid label, got: `%s`", lineNumber, label);
            }
            state->hasError = true;
            continue;
        }

        if (tryGetDirective(directive, line)){
            switch (directiveTypeFromString(directive)){
                case DIRECTIVE_TYPE_ENTRY:
                    if (hasLabel) {
                        logWarning("line %3d: entry directive should not have a label. got label `%s`",
                                   lineNumber, label);
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
            if (hasLabel && !symbolsSetInsert(state->symbols, SYMBOL_TYPE_CODE, label, state->IC)){
                logWarning("line %3d: already have symbol `%s`, ignoring", lineNumber, label);
            }
            handleOperation(state, lineNumber, operation, operationArguments);
        } else {
            logError("line %3d: unknown error, unable to parse", lineNumber);
            state->hasError = true;
        }
    }

    if (!state->hasError){
        updateDataSymbolsWithInstructionsCounter(state);
    }
}