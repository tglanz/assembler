#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

typedef enum {
    ADDRESS_TYPE_IMMEDIATE   = 0x1,
    ADDRESS_TYPE_DIRECT      = 0x3,
    ADDRESS_TYPE_REGISTER    = 0x5
} InstructionAddressType;

typedef enum {
    OPCODE_MOV = 0x0,
    OPCODE_CMP = 0x1,
    OPCODE_ADD = 0x2,
    OPCODE_SUB = 0x3,
    OPCODE_NOT = 0x4,
    OPCODE_CLR = 0x5,
    OPCODE_LEA = 0x6,
    OPCODE_INC = 0x7,
    OPCODE_DEC = 0x8,
    OPCODE_JMP = 0x9,
    OPCODE_BNE = 0xa,
    OPCODE_RED = 0xb,
    OPCODE_PRN = 0xc,
    OPCODE_JSR = 0xd,
    OPCODE_RTS = 0xe,
    OPCODE_STP = 0xf
} InstructionOperationCode;

typedef struct {
    const char * name;
    const InstructionOperationCode code;
    const unsigned int operandsCount;
} InstructionModel;

const InstructionModel INSTRUCTIONS_TABLE[] = {
    { "mov", OPCODE_MOV, 2 },
    { "cmp", OPCODE_CMP, 2 },
    { "add", OPCODE_ADD, 2 },
    { "sub", OPCODE_SUB, 2 },
    { "not", OPCODE_NOT, 1 },
    { "clr", OPCODE_CLR, 1 },
    { "lea", OPCODE_LEA, 2 },
    { "inc", OPCODE_INC, 1 },
    { "dec", OPCODE_DEC, 1 },
    { "jmp", OPCODE_JMP, 1 },
    { "bne", OPCODE_BNE, 1 },
    { "red", OPCODE_RED, 1 },
    { "prn", OPCODE_PRN, 1 },
    { "jsr", OPCODE_JSR, 1 },
    { "rts", OPCODE_RTS, 0 },
    { "stop", OPCODE_STP, 0 }
};

#endif /* __INSTRUCTIONS_H__ */