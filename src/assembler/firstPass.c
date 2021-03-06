#include "assembler/firstPass.h"

void firstPassHandleExternDirective(AssemblyState * state){
    char arguments[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];
    int argIndex = 0;

    /* get the labels the directive refers to */
    if (!tryGetDirectiveArgs(arguments, state->line) || strlen(arguments) == 0){
        logError("@%-3d: missing arguments for extern directive", state->lineNumber);
        state->hasError = true;
        return;
    }
    
    /* iterate the labels to be externed */
    while (getSplitComponent(argument, arguments, " ", argIndex)){
        if (!symbolsTableInsert(state->symbols, SYMBOL_TYPE_NONE, argument, 0)){
            logError("@%-3d: cannot mark as external, already have symbol: `%s`",
                     state->lineNumber, argument);
            state->hasError = true;
        } else {
            /* set relevant flags */
            symbolsTableFlag(state->symbols, argument, SYMBOL_FLAG_EXTERNAL);
        }
        ++argIndex;
    }
}

void firstPassHandleStringDirective(AssemblyState * state){
    uint idx;
    Word word;
    char stringData[MAX_LINE_LENGTH];
    bool isValid = false;

    /* detect issues */
    if (!tryGetDirectiveArgs(stringData, state->line) || strlen(stringData) == 0){
        logError("@%-3d: invalid string directive, expected arguments",
                 state->lineNumber);
    } else if (stringData[0] != '\"'){
        logError("@%-3d: invalid string directive, first, and only argument must be a string. got: `%s`",
                 state->lineNumber, stringData);
    } else if (countCharacterOccurrences(state->line, 0, '\"') == 1) {
        logError("@%-3d: invalid string directive, missing closing tag. got: `%s`",
                 state->lineNumber, stringData);
    } else if (countCharacterOccurrences(state->line, 0, '\"') > 2){
        logError("@%-3d: invalid string directive, expected exactly one string argument. got: `%s`",
                 state->lineNumber, stringData);
    } else {
        isValid = true;
    }

    if (!isValid){
        state->hasError = true;
        return;
    }
    
    /* no issues thus far, break the string into words and add them to the data */
    getSplitComponent(stringData, state->line, "\"", 1);
    for (idx = 0; idx < strlen(stringData); ++idx){
        word.raw = stringData[idx];
        wordsVectorAppend(state->data, word);
    }

    /* append null terminator */
    word.raw = 0;
    wordsVectorAppend(state->data, word);
    state->DC += strlen(stringData) + 1;
}

void firstPassHandleDataDirective(AssemblyState * state){
    char arguments[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];
    int argIndex = 0;
    int integer;
    Word word;

    /* extract the arguments of the directive */
    if (!tryGetDirectiveArgs(arguments, state->line) || strlen(arguments) == 0){
        logWarning("@%-3d: no arguments for data directive, skipping", state->lineNumber);
        return;
    }

    /* iterate all argument values, and add them to the data */
    while (getSplitComponent(argument, arguments, ",", argIndex++)){
        if (sscanf(argument, "%d", &integer) != 1){
            logError("@%-3d: cannot parse argument `%s` to integer",
                     state->lineNumber, argument);
            state->hasError = true;
            return;
        }

        word.raw = integer;
        wordsVectorAppend(state->data, word);
        ++state->DC;
    }
}

bool setInstructionAddressTypes(
    AssemblyState * state, const InstructionModel * instructionModel, InstructionWord * instructionWord, string arguments){
    int idx, operandsCount;
    char argument[MAX_LINE_LENGTH];
    OperandAddressType addressType;

    /* Set the instruction address types fields, according to the arguments of an operation */
    
    /* check the actual arguments given */
    if (strlen(arguments) == 0){
        operandsCount = 0;
    } else {
        operandsCount = countCharacterOccurrences(arguments, 0, ',') + 1;
    }

    /* compare the actual operands count to the model */
    if (operandsCount != getModelOperandsCount(instructionModel)){
        logError("@%-3d: invalid argument count for operation `%s`. expected %d",
                 instructionModel->operation, operandsCount);
        return false;
    }

    /**
     * iterate all of the arguments and for each one, figure out it's address type,
     * and then set the corresponding source/destination address type in the instructon word
     */
    for (idx = 0; idx < operandsCount; ++idx){
        /* get the argument */
        getSplitComponent(argument, arguments, ",", idx);
        addressType = oeprandStringToAddressType(argument);

        if (addressType == ADDRESS_TYPE_NONE){
            logError("@%-3d: invalid argument `%s`, unknown address type, arguments: `%s`",
                        state->lineNumber, argument, arguments);
            return false;
        }

        if (idx == 1 || instructionModel->addressTypes[0] == ADDRESS_TYPE_NONE){
            instructionWord->fields.destinationAddressType = addressType;
            
        } else if (idx == 0){
            instructionWord->fields.sourceAddressType = addressType;
        }
    }    

    return true;
}

void firstPassHandleOperation(AssemblyState * state, string operation, string arguments) {
    const InstructionModel * instructionModel;
    InstructionWord instructionWord;
    int idx, dataWordsCount;

    /* handle a line of operation */

    /* acquire the model of the opration */
    instructionModel = findInstructionModel(operation);
    if (instructionModel == NULL){
        logError("@%-3d: unknown operation: `%s`", operation);
        state->hasError = true;
        return;
    }

    /* start constructing the instruction */
    instructionWord.word.raw = 0;

    /* set address types */
    if (!setInstructionAddressTypes(state, instructionModel, &instructionWord, arguments)){
        logError("@%-3d: invalid operand address types", state->lineNumber);
        state->hasError = true;
        return;
    }

    /* append instruction, and placholders for operands to the instructions */
    instructionWord.fields.opcode = instructionModel->code;
    wordsVectorAppend(state->code, instructionWord.word);

    dataWordsCount = getDataWordsCount(instructionWord.fields.sourceAddressType,
                                       instructionWord.fields.destinationAddressType);
    for (idx = 0; idx < dataWordsCount; ++idx){
        instructionWord.word.raw = 0;
        wordsVectorAppend(state->code, instructionWord.word);
    }

    state->IC += 1 + dataWordsCount;
}

void updateDataSymbolsWithInstructionsCounter(AssemblyState * state) {
    uint idx, newValue;
    Symbol * symbol;

    /** for all data symbols,
     * increment their addresses according to the instructions counter
     */
    for (idx = 0; idx < state->symbols->size; ++idx){
        symbol = &state->symbols->data[idx];
        if (symbol->type == SYMBOL_TYPE_DATA){
            newValue = symbol->value + state->IC + INSTRUCTIONS_OFFSET;
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

    /**
     * Read all lines in the file, and detect the type - oepration or directive
     * Handle each one seperately
     */
    while (readLine(sourceFile, state->line)){
        ++state->lineNumber;
        logDebug("first pass, read line: %d: `%s`", state->lineNumber, state->line);

        if (!isMeaningfulLine(state->line)){
            continue;
        }

        /* extract labels from the line */
        hasLabel = tryGetLabel(label, state->line);
        if (hasLabel && !isValidLabel(label)){
            if (strlen(label) > 0 && isWhitespaceCharacter(label[0])){
                logError("@%-3d: invalid label, got: `%s`. hint: labels must not start with a whitespace!",
                         state->lineNumber, label);
            } else {
                logError("@%-3d: invalid label, got: `%s`", state->lineNumber, label);
            }
            state->hasError = true;
            continue;
        }

        if (tryGetDirective(directive, state->line)){
            /* classify directive and handle it */
            switch (directiveTypeFromString(directive)){
                case DIRECTIVE_TYPE_ENTRY:
                    if (hasLabel) {
                        logWarning("@%-3d: entry directive should not have a label. got label `%s`",
                                   state->lineNumber, label);
                    }
                    break;
                case DIRECTIVE_TYPE_EXTERN:
                    firstPassHandleExternDirective(state);
                    break;
                case DIRECTIVE_TYPE_STRING:
                    if (hasLabel)
                        symbolsTableInsert(state->symbols, SYMBOL_TYPE_DATA, label, state->DC);
                    firstPassHandleStringDirective(state);
                    break;
                case DIRECTIVE_TYPE_DATA:
                    if (hasLabel)
                        symbolsTableInsert(state->symbols, SYMBOL_TYPE_DATA, label, state->DC);
                    firstPassHandleDataDirective(state);
                    break;
                case DIRECTIVE_TYPE_INVALID:
                    logError("@%-3d: invalid directive: `%s`", state->lineNumber, directive);
                    state->hasError = true;
                    break;
            }
        } else if (tryGetOperation(operation, operationArguments, state->line, hasLabel)){
            /* handle the operation line */
            if (hasLabel && !symbolsTableInsert(state->symbols, SYMBOL_TYPE_CODE, label, state->IC + INSTRUCTIONS_OFFSET)){
                logWarning("@%-3d: already have symbol `%s`, ignoring", state->lineNumber, label);
            }
            firstPassHandleOperation(state, operation, operationArguments);
        } else {
            /* invalid line, hopefuly we don't have a parsing bug */
            logError("@%-3d: unknown error, unable to parse", state->lineNumber);
            state->hasError = true;
        }
    }

    if (!state->hasError){
        /* update all of the data symbols, to match the references according to the instructions state */
        updateDataSymbolsWithInstructionsCounter(state);
    }
}