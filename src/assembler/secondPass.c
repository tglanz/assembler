#include "assembler/secondPass.h"

void secondPassHandleEntryDirective(AssemblyState * state) {
    char arguments[MAX_LINE_LENGTH];

    /**
     * in the second pass we already encountered all available symbols,
     * so we can flag them as entry if required.
     */

    if (!tryGetDirectiveArgs(arguments, state->line)) {
        logError("@%-3d: invalid entry directive, expected an argument",
                 state->lineNumber);
        state->hasError = true;
        return;
    }

    if (!isValidLabel(arguments)){
        logError("@%-3d: unable to set entry for `%s`. it is not a valid label",
                 state->lineNumber, arguments);
        state->hasError = true;
        return;
    }

    if (!symbolsTableFlag(state->symbols, arguments, SYMBOL_FLAG_ENTRY)){
        logError("@%-3d: unable to set entry for `%s`. are you sure it exists?",
                 state->lineNumber, arguments);
        state->hasError = true;
        return;
    }
}

void secondPassHandleOperation(AssemblyState * state, string operation, string arguments){
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

    /**
     * Now, we can fill the missing data left by the first pass.
     * Encode the operands and set external labels addresses
     */

    instructionModel = findInstructionModel(operation);
    operandsCount = getModelOperandsCount(instructionModel);

    instructionWord.word = wordsVectorGet(state->code, state->IC);

    addressTypes[0] = instructionWord.fields.sourceAddressType;
    addressTypes[1] = instructionWord.fields.destinationAddressType;

    /* point IC to the data words */
    state->IC++;

    /* only special case is when both addresstypes are registers */
    if (addressTypes[0] == ADDRESS_TYPE_REGISTER && addressTypes[1] == ADDRESS_TYPE_REGISTER){
        getSplitComponent(argument, arguments, ",", 0);
        registerOperandWord.fields.sourceRegister = registerIndexFromArgumentString(argument);
        getSplitComponent(argument, arguments, ",", 1);
        registerOperandWord.fields.destinationRegister = registerIndexFromArgumentString(argument);
        registerOperandWord.fields.encodingType = ENCODING_TYPE_ABSOLUTE;
        wordsVectorSet(state->code, state->IC, registerOperandWord.word);
        if (registerOperandWord.fields.sourceRegister == INVALID_REGISTER){
            logError("@%-3d: invalid source register in arguments: `%s`",
                     state->lineNumber, arguments);
            state->hasError = true;
        }
        if (registerOperandWord.fields.destinationRegister == INVALID_REGISTER){
            logError("@%-3d: invalid destination register in arguments: `%s`",
                     state->lineNumber, arguments);
            state->hasError = true;
        }
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
                    /* Encode immediate operand */
                    if (sscanf(argument, "%d", &value) != 1){
                        logError("@%-3d: expected immediate value, but failed to parse argument at position: %d",
                                 state->lineNumber, idx);
                        state->hasError = true;
                    } else {
                        immediateOperandWord.fields.immediateValue = (uint)value;
                        immediateOperandWord.fields.encodingType = ENCODING_TYPE_ABSOLUTE;
                        word = immediateOperandWord.word;
                    }
                    break;
                case ADDRESS_TYPE_DIRECT:
                    /* Encode direct operand */
                    symbol = symbolsTableFind(state->symbols, argument);
                    if (symbol == NULL){
                        logError("@%-3d: expected symbol, but it is not defined in the program (or extern). searched for `%s`",
                                 state->lineNumber, argument);
                        state->hasError = true;
                    } else if (has_flag(SYMBOL_FLAG_EXTERNAL, symbol->flags)){
                        /* We have sufficient info for the external label, set it accordingly to the instruction */
                        directOperandWord.fields.address = 0;
                        directOperandWord.fields.encodingType = ENCODING_TYPE_EXTERNAL;
                        word = directOperandWord.word;
                        symbolsTableAppend(state->externalSymbols,
                                           SYMBOL_TYPE_NONE,
                                           symbol->key,
                                           state->IC + INSTRUCTIONS_OFFSET);
                    } else {
                        directOperandWord.fields.address = symbol->value;
                        directOperandWord.fields.encodingType = ENCODING_TYPE_RELOCATBALE;
                        word = directOperandWord.word;
                    }
                    break;
                case ADDRESS_TYPE_REGISTER:
                    /* Encode register operand */
                    registerOperandWord.word.raw = 0;
                    value = registerIndexFromArgumentString(argument);
                    if (value == INVALID_REGISTER){
                        logError("@%-3d: invalid register: `%s` in arguments: `%s`",
                                state->lineNumber, argument, arguments);
                        state->hasError = true;
                    }
                    registerOperandWord.fields.encodingType = ENCODING_TYPE_ABSOLUTE;
                    if (idx == 0){
                        registerOperandWord.fields.sourceRegister = value;
                    } else {
                        registerOperandWord.fields.destinationRegister = value;
                    }
                    word = registerOperandWord.word;
                    break;
                default:
                    logError("@%-3d: unknown address type: %d", state->lineNumber, addressTypes[idx]);
                    state->hasError = true;
            }

            /* set the word */
            wordsVectorSet(state->code, state->IC, word);
            state->IC++;
        }
    }
}

void runSecondPass(AssemblyState * state, SourceFile * sourceFile){
    /* Decleration */
    bool hasLabel;

    char label[MAX_LINE_LENGTH];
    char directive[MAX_LINE_LENGTH];
    char operation[MAX_LINE_LENGTH];
    char arguments[MAX_LINE_LENGTH];

    /* Defaults */
    state->IC = 0;
    state->lineNumber = 0;

    while (readLine(sourceFile, state->line)){
        /* remove the newline at the end */
        ++state->lineNumber;

        logDebug("second pass, read line: %d: `%s`", state->lineNumber, state->line);
        if (!isMeaningfulLine(state->line)){
            continue;
        }

        hasLabel = tryGetLabel(label, state->line);

        if (tryGetDirective(directive, state->line)){
            /* we need only to handle entry directives in this pass */
            if (directiveTypeFromString(directive) == DIRECTIVE_TYPE_ENTRY){
                secondPassHandleEntryDirective(state);
            }
        } else if (tryGetOperation(operation, arguments, state->line, hasLabel)){
            /* it's an operation, lets handle it */
            secondPassHandleOperation(state, operation, arguments);
        }
    }
}