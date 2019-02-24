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

const InstructionModel * findInstructionModel(const char * operation){
    int idx;
    for (idx = 0; idx < sizeof(INSTRUCTIONS_TABLE) / sizeof(InstructionModel); ++idx){
        if (strcmp(operation, INSTRUCTIONS_TABLE[idx].operation) == 0){
            return &INSTRUCTIONS_TABLE[idx];
        }
    }

    return NULL;
}

unsigned int getModelOperandsCount(const InstructionModel * model){
    return (model->addressTypes[0] & ADDRESS_TYPE_IMMEDIATE) + 
           (model->addressTypes[1] & ADDRESS_TYPE_IMMEDIATE);
}

OperandAddressType oeprandStringToAddressType(const char * argument){
    int tmp;

    if (strlen(argument) != 0){
        if (argument[0] == REGISTER_ADDRESS_PREFIX){
            return ADDRESS_TYPE_REGISTER;
        }

        if (sscanf(argument, "%d", &tmp) == 1){
            return ADDRESS_TYPE_IMMEDIATE;
        }

        if (isValidLabel(argument)){
            return ADDRESS_TYPE_DIRECT;
        }
    }

    return ADDRESS_TYPE_NONE;
}

int getDataWordsCount(OperandAddressType sourceAddressType, OperandAddressType destinationAddressType){
    int count = 0;

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