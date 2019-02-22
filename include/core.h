#ifndef __CORE_H__
#define __CORE_H__

#define WORD_SIZE (12)

#define ENCODING_TYPE_WIDTH (2)
#define OPCODE_WIDTH (4)
#define OPERAND_WIDTH (3)

typedef struct {
    int raw: WORD_SIZE;
} Word;

typedef enum {
    true = 1,
    false = 0
} bool;

enum {
    EXIT_CODE_SUCCESS = 0,
    EXIT_CODE_UNKNOWN = -1,
    EXIT_CODE_FATAL = -2
};

enum {
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
};

/* Encoding type (A,R,E bits) */
enum {
    ENCODING_TYPE_ABSOLUTE    = 0x0,
    ENCODING_TYPE_EXTERNAL    = 0x1,
    ENCODING_TYPE_RELOCATBALE = 0x3
};

enum {
    ADDRESS_TYPE_IMMEDIATE   = 0x1,
    ADDRESS_TYPE_DIRECT      = 0x3,
    ADDRESS_TYPE_REGISTER    = 0x5
};

typedef struct {
    unsigned int kind:          ENCODING_TYPE_WIDTH; /* [1: 0] */
    unsigned int destination:   OPERAND_WIDTH;       /* [4: 2] */
    unsigned int opcode:        OPCODE_WIDTH;        /* [8: 5] */
    unsigned int source:        OPERAND_WIDTH;       /* [11:9] */
} MainInstruction;

#endif /* __CORE_H__ */