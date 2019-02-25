#include "assembler/firstPass.h"

void handleExternDirective(AssemblyState * state){
    char arguments[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];
    int argIndex = 0;

    if (!tryGetDirectiveArgs(arguments, state->line) || strlen(arguments) == 0){
        logError("line %3d: missing arguments for extern directive", state->lineNumber);
        state->hasError = true;
        return;
    }
    
    while (getSplitComponent(argument, arguments, " ", argIndex)){
        if (!symbolsSetInsert(state->symbols, SYMBOL_TYPE_EXTERN, argument, 0)){
            logWarning("line %3d: already have symbol: %s",
                       state->lineNumber, argument);
        }
        ++argIndex;
    }
}

void handleEntryDirective(AssemblyState * state){
    char arguments[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];
    int argIndex = 0;

    if (!tryGetDirectiveArgs(arguments, state->line) || strlen(arguments) == 0){
        logError("line %3d: missing arguments for entry directive", state->lineNumber);
        state->hasError = true;
        return;
    }

    while (getSplitComponent(argument, arguments, " ", argIndex)){
        if (argIndex > 0){
            logWarning("line %3d: entry directive can have only a single label as argument, ignoring `%s`",
                       state->lineNumber, argument);
        } else if (!symbolsSetInsert(state->symbols, SYMBOL_TYPE_ENTRY, argument, 0)){
            logWarning("line %3d: duplicate symbol: `%s`", state->lineNumber, argument);
        }
        ++argIndex;
    }
}

void handleStringDirective(AssemblyState * state){
    int cursor;
    Word word;
    char stringData[MAX_LINE_LENGTH];

    if (countCharacterOccurrences(state->line, 0, '\"') != 2){
        logError("line %3d: invalid string directive, expected exactly one argument", state->lineNumber);
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

void handleDataDirective(AssemblyState * state){
    char arguments[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];
    int argIndex = 0;
    int integer;
    Word word;

    if (!tryGetDirectiveArgs(arguments, state->line) || strlen(arguments) == 0){
        logWarning("line %3d: no arguments for data directive, skipping", state->lineNumber);
        return;
    }

    while (getSplitComponent(argument, arguments, ",", argIndex++)){
        if (sscanf(argument, "%d", &integer) != 1){
            logError("line %3d: cannot parse argument `%s` to integer",
                     state->lineNumber, argument);
            state->hasError = true;
            return;
        }

        word.raw = integer;
        wordsVectorAppend(state->data, word);
        ++state->DC;
    }
}

void setInstructionAddressTypes(
    AssemblyState * state, const InstructionModel * instructionModel, InstructionWord * instructionWord, const char * arguments){
    int idx, operandsCount;
    char argument[MAX_LINE_LENGTH];
    
    /* check the actual arguments given */
    if (strlen(arguments) == 0){
        operandsCount = 0;
    } else {
        operandsCount = countCharacterOccurrences(arguments, 0, ',') + 1;
    }

    /* compare the actual operands count to the model */
    if (operandsCount != getModelOperandsCount(instructionModel)){
        logError("line %3d: invalid argument count for operation `%s`. expected %d",
                 instructionModel->operation, operandsCount);
        state->hasError = true;
        return;
    }

    /**
     * iterate all of the arguments and for each one, figure out it's address type,
     * and then set the corresponding source/destination address type in the instructon word
     */
    for (idx = 0; idx < operandsCount; ++idx){
        /* get the argument */
        getSplitComponent(argument, arguments, ",", idx);
        if (idx == 1 || instructionModel->addressTypes[0] == ADDRESS_TYPE_NONE){
            instructionWord->fields.destinationAddressType = oeprandStringToAddressType(argument);
        } else if (idx == 0){
            instructionWord->fields.sourceAddressType = oeprandStringToAddressType(argument);
        }
    }    
}

int ids = 0;

void handleOperation(AssemblyState * state, const char * operation, const char * arguments) {
    const InstructionModel * instructionModel;
    InstructionWord instructionWord;
    int idx, dataWordsCount;

    instructionModel = findInstructionModel(operation);
    if (instructionModel == NULL){
        logError("line %3d: unknown operation: `%s`", operation);
        state->hasError = true;
        return;
    }

    instructionWord.word.raw = 0;

    setInstructionAddressTypes(state, instructionModel, &instructionWord, arguments);
    if (state->hasError){
        logError("line %3d: invalid operand address types", state->lineNumber);
        return;
    }

    instructionWord.fields.opcode = instructionModel->code;
    wordsVectorAppend(state->instructions, instructionWord.word);

    dataWordsCount = getDataWordsCount(instructionWord.fields.sourceAddressType,
                                       instructionWord.fields.destinationAddressType);
    for (idx = 0; idx < dataWordsCount; ++idx){
        instructionWord.word.raw = 0xaaa;
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
    char operation[MAX_LINE_LENGTH];
    char operationArguments[MAX_LINE_LENGTH];
    char directive[MAX_LINE_LENGTH];

    bool hasLabel;
    char label[MAX_LINE_LENGTH];

    /* Defaults */
    state->lineNumber = 0;
    while (readLine(sourceFile, state->line)){
        ++state->lineNumber;
        logDebug("first pass, read line: line %3d: %s", state->lineNumber, state->line);

        if (!isMeaningfulLine(state->line)){
            continue;
        }

        hasLabel = tryGetLabel(label, state->line);
        if (hasLabel && !isValidLabel(label)){
            if (strlen(label) > 0 && isWhitespaceCharacter(label[0])){
                logError("line %3d: invalid label, got: `%s`. hint: labels must not start with a whitespace!",
                         state->lineNumber, label);
            } else {
                logError("line %3d: invalid label, got: `%s`", state->lineNumber, label);
            }
            state->hasError = true;
            continue;
        }

        if (tryGetDirective(directive, state->line)){
            switch (directiveTypeFromString(directive)){
                case DIRECTIVE_TYPE_ENTRY:
                    if (hasLabel) {
                        logWarning("line %3d: entry directive should not have a label. got label `%s`",
                                   state->lineNumber, label);
                    }
                    handleEntryDirective(state);
                    break;
                case DIRECTIVE_TYPE_EXTERN:
                    handleExternDirective(state);
                    break;
                case DIRECTIVE_TYPE_STRING:
                    if (hasLabel)
                        symbolsSetInsert(state->symbols, SYMBOL_TYPE_DATA, label, state->DC);
                    handleStringDirective(state);
                    break;
                case DIRECTIVE_TYPE_DATA:
                    if (hasLabel)
                        symbolsSetInsert(state->symbols, SYMBOL_TYPE_DATA, label, state->DC);
                    handleDataDirective(state);
                    break;
                case DIRECTIVE_TYPE_INVALID:
                    logError("line %3d: invalid directive: `%s`", state->lineNumber, directive);
                    state->hasError = true;
                    break;
            }
        } else if (tryGetOperation(operation, operationArguments, state->line, hasLabel)){
            if (hasLabel && !symbolsSetInsert(state->symbols, SYMBOL_TYPE_CODE, label, state->IC)){
                logWarning("line %3d: already have symbol `%s`, ignoring", state->lineNumber, label);
            }
            handleOperation(state, operation, operationArguments);
        } else {
            logError("line %3d: unknown error, unable to parse", state->lineNumber);
            state->hasError = true;
        }
    }

    if (!state->hasError){
        updateDataSymbolsWithInstructionsCounter(state);
    }
}