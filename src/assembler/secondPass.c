#include "assembler/secondPass.h"

void handleOperationInSecondPass(AssemblyState * state, unsigned int lineNumber, const char * operation, const char * arguments){
    int idx, operandsCount, value, tmpi;
    const InstructionModel * instructionModel;
    char argument[MAX_LINE_LENGTH];
    const Symbol * symbol;

    InstructionWord instructionWord;
    ImmediateOperandWord immediateOperandWord;
    DirectOperandWord directOperandWord;
    RegisterOperandWord registerOperandWord;
    OperandAddressType addressTypes[2];

    instructionModel = findInstructionModel(operation);
    operandsCount = getModelOperandsCount(instructionModel);

    instructionWord.word = wordsVectorGet(state->instructions, state->IC);

    addressTypes[0] = instructionWord.fields.sourceAddressType;
    addressTypes[1] = instructionWord.fields.destinationAddressType;

    for (idx = 0; idx < operandsCount; ++idx){

        if (!getSplitComponent(argument, arguments, ",", idx)){
            logError("line %3d: cannot find argument at position: %d", lineNumber, idx);
            state->hasError = true;
            continue;
        }

        if (idx == 0 && addressTypes[0] == ADDRESS_TYPE_NONE){
            ++idx;
        }

        switch (addressTypes[idx]){

            case ADDRESS_TYPE_IMMEDIATE:
                if (sscanf("%d", argument, &value) != 1){
                    logError("line %3d: expected immediate value, but failed to parse argument at position: %d",
                             lineNumber, idx);
                    state->hasError = true;
                } else {
                    immediateOperandWord.fields.immediateValue = (unsigned int)value;
                    wordsVectorSet(state->instructions, state->IC + 1 + idx, immediateOperandWord.word);
                }
                break;
            case ADDRESS_TYPE_DIRECT:
                symbol = symbolsSetFind(state->symbols, argument);
                if (symbol == NULL){
                    logError("line %3d: expected symbol, but it is not defined in the program (or extern). searched for `%s`",
                             lineNumber, argument);
                    state->hasError = true;
                } else {
                    directOperandWord.fields.address = symbol->value;
                    wordsVectorSet(state->instructions, state->IC + 1 + idx, directOperandWord.word);
                }
                break;
            case ADDRESS_TYPE_REGISTER:
                value = registerIndexFromArgumentString(argument);
                if (value == INVALID_REGISTER){
                    logError("line %3d: unrecognized register: `%s`", lineNumber, argument);
                    state->hasError = true;
                } else if (idx == 0){
                    registerOperandWord.word = wordsVectorGet(state->instructions, state->IC + 1);
                    registerOperandWord.fields.sourceRegister = value;
                    wordsVectorSet(state->instructions, state->IC + 1, registerOperandWord.word);
                } else if (idx == 1){
                    tmpi = state->IC + 1;
                    if (addressTypes[0] != ADDRESS_TYPE_REGISTER){
                        tmpi++;
                    }
                    registerOperandWord.word = wordsVectorGet(state->instructions, tmpi);
                    registerOperandWord.fields.sourceRegister = value;
                    wordsVectorSet(state->instructions, tmpi, registerOperandWord.word);
                }
                break;
            default:
                logError("line %3d: unrecognized address type: %d", lineNumber, addressTypes[idx]);
                state->hasError = true;
        }
    }

    printf("IC: %s, %d ", instructionModel->operation, state->IC);
    state->IC += 1 + getDataWordsCount(addressTypes[0], addressTypes[1]);
    printf("-> %d\n", state->IC);
}

void runSecondPass(AssemblyState * state, SourceFile * sourceFile){
    /* Decleration */
    unsigned int lineNumber;

    char line[MAX_LINE_LENGTH];
    char dump[MAX_LINE_LENGTH];

    bool hasLabel;
    bool hasDirective;

    char operation[MAX_LINE_LENGTH];
    char arguments[MAX_LINE_LENGTH];

    /* Defaults */
    lineNumber = 0;
    state->IC = 0;

    while (readLine(sourceFile, line)){
        /* remove the newline at the end */
        ++lineNumber;

        logDebug("second pass, read line: line %3d: %s", lineNumber, line);

        if (!isMeaningfulLine(line)){
            continue;
        }

        hasLabel = tryGetLabel(dump, line);
        hasDirective = tryGetDirective(dump, line);

        if (!hasDirective && tryGetOperation(operation, arguments, line, hasLabel)){
            handleOperationInSecondPass(state, lineNumber, operation, arguments);
        }
    }
}