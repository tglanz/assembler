#include "assembler/secondPass.h"

void handleOperationInSecondPass(AssemblyState * state, unsigned int lineNumber, const char * operation, const char * arguments){
    int idx, operandsCount, value;
    const InstructionModel * instructionModel;
    char argument[MAX_LINE_LENGTH];
    const Symbol * symbol;

    Word word;
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

    /* point IC to the data words */
    state->IC++;

    /* only special case is when both addresstypes are registers */
    if (addressTypes[0] == ADDRESS_TYPE_REGISTER && addressTypes[1] == ADDRESS_TYPE_REGISTER){
        getSplitComponent(argument, argument, ",", 0);
        registerOperandWord.fields.sourceRegister = registerIndexFromArgumentString(argument);
        getSplitComponent(argument, arguments, ",", 1);
        registerOperandWord.fields.destinationRegister = registerIndexFromArgumentString(argument);
        registerOperandWord.fields.encodingType = ENCODING_TYPE_ABSOLUTE;
        wordsVectorSet(state->instructions, state->IC, registerOperandWord.word);
        state->IC++;
    } else {
        for (idx = 0; idx < operandsCount; ++idx){
            getSplitComponent(argument, arguments, ",", idx);
            if (idx == 0 && addressTypes[0] == ADDRESS_TYPE_NONE){
                /* in case first address type is 0, then this is destination operand */
                ++idx;
            }

            switch (addressTypes[idx]){
                case ADDRESS_TYPE_IMMEDIATE:
                    if (sscanf(argument, "%d", &value) != 1){
                        logError("line %3d: expected immediate value, but failed to parse argument at position: %d",
                                lineNumber, idx);
                        state->hasError = true;
                    } else {
                        immediateOperandWord.fields.immediateValue = (unsigned int)value;
                        immediateOperandWord.fields.encodingType = ENCODING_TYPE_ABSOLUTE;
                        word = immediateOperandWord.word;
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
                        directOperandWord.fields.encodingType = ENCODING_TYPE_RELOCATBALE;
                        word = directOperandWord.word;
                    }
                    break;
                case ADDRESS_TYPE_REGISTER:
                    registerOperandWord.word.raw = 0;
                    value = registerIndexFromArgumentString(argument);
                    registerOperandWord.fields.encodingType = ENCODING_TYPE_ABSOLUTE;
                    if (idx == 0){
                        registerOperandWord.fields.sourceRegister = value;
                    } else {
                        registerOperandWord.fields.destinationRegister = value;
                    }
                    word = registerOperandWord.word;
                    break;
                default:
                    logError("line %3d: unknown address type: %d", state->lineNumber, addressTypes[idx]);
                    state->hasError = true;
            }

            /* set the word */
            wordsVectorSet(state->instructions, state->IC, word);
            state->IC++;
        }
    }
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