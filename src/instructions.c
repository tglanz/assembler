#include "instructions.h"

/* Compile time informaion on each available operation */
const InstructionModel INSTRUCTIONS_TABLE[] = {
    { 
        "mov", 0x0, {
        ADDRESS_TYPE_IMMEDIATE | ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },

    {
        "cmp", 0x1, {
        ADDRESS_TYPE_IMMEDIATE | ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER,
        ADDRESS_TYPE_IMMEDIATE | ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "add", 0x2, {
        ADDRESS_TYPE_IMMEDIATE | ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "sub", 0x3, {
        ADDRESS_TYPE_IMMEDIATE | ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "not", 0x4, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "clr", 0x5, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "lea", 0x6, {
        ADDRESS_TYPE_DIRECT,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "inc", 0x7, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "dec", 0x8, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "jmp", 0x9, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "bne", 0xA, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "red", 0xB, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "prn", 0xC, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_IMMEDIATE | ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "jsr", 0xD, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_IMMEDIATE | ADDRESS_TYPE_DIRECT | ADDRESS_TYPE_REGISTER },
    },
    {
        "rts", 0xE, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_NONE },
    },
    {
        "stop", 0xF, {
        ADDRESS_TYPE_NONE,
        ADDRESS_TYPE_NONE },
    }
};

const InstructionModel * findInstructionModel(string operation){
    int idx;

    /* just search for a model with the same name as operation */
    for (idx = 0; idx < sizeof(INSTRUCTIONS_TABLE) / sizeof(InstructionModel); ++idx){
        if (strcmp(operation, INSTRUCTIONS_TABLE[idx].operation) == 0){
            return &INSTRUCTIONS_TABLE[idx];
        }
    }

    return NULL;
}

uint getModelOperandsCount(const InstructionModel * model){
    /* for each address type, check its lsbit */
    return (model->addressTypes[0] & ADDRESS_TYPE_IMMEDIATE) + 
           (model->addressTypes[1] & ADDRESS_TYPE_IMMEDIATE);
}

OperandAddressType oeprandStringToAddressType(string argument){
    int tmp;

    /* detect the format of the argument */

    if (strlen(argument) != 0){

        /* if register, should start with @r */
        if (substringIndex(argument, REGISTER_ADDRESS_PREFIX) == 0){
            /* TODO: what about @rbcd for example */
            return ADDRESS_TYPE_REGISTER;
        }

        /* if its a number, its an immediate */
        if (sscanf(argument, "%d", &tmp) == 1){
            return ADDRESS_TYPE_IMMEDIATE;
        }

        /* can it refer to a label? */
        if (isValidLabel(argument)){
            /* its direct */
            return ADDRESS_TYPE_DIRECT;
        }
    }

    /* no valid result */
    return ADDRESS_TYPE_NONE;
}

uint getDataWordsCount(OperandAddressType sourceAddressType, OperandAddressType destinationAddressType){
    int count = 0;

    /* count how many data words required according the the address types */

    if (sourceAddressType != ADDRESS_TYPE_NONE){
        ++count;
    }

    if (destinationAddressType == ADDRESS_TYPE_DIRECT || destinationAddressType == ADDRESS_TYPE_IMMEDIATE){
        ++count;
    } else if (destinationAddressType == ADDRESS_TYPE_REGISTER && sourceAddressType != ADDRESS_TYPE_REGISTER){
        ++count;
    }

    return count;
}

uint registerIndexFromArgumentString(string argumentString){
    uint registerIndex;

    /* try parse it as @r{number} */
    if (sscanf(argumentString + strlen(REGISTER_ADDRESS_PREFIX), "%u", &registerIndex) != 1 ||
        registerIndex < REGISTER_MIN ||
        registerIndex > REGISTER_MAX){

        registerIndex = INVALID_REGISTER;
    }

    return registerIndex;
}