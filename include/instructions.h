#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include <string.h>

#include "core.h"
#include "log.h"
#include "parsing.h"
#include "words.h"

#define REGISTER_ADDRESS_PREFIX ('@')
#define REGISTER_ARGUMENT_INDEX_OFFSET (2)
#define REGISTER_MIN (0)
#define REGISTER_MAX (7)
#define INVALID_REGISTER (REGISTER_MAX + 1)

typedef enum {
    ADDRESS_TYPE_NONE        = 0x0,
    ADDRESS_TYPE_IMMEDIATE   = 0x1,
    ADDRESS_TYPE_DIRECT      = 0x3,
    ADDRESS_TYPE_REGISTER    = 0x5
} OperandAddressType;

/* Encoding type (A,R,E bits) */
typedef enum {
    ENCODING_TYPE_ABSOLUTE    = 0x0,
    ENCODING_TYPE_EXTERNAL    = 0x1,
    ENCODING_TYPE_RELOCATBALE = 0x2
} InstructionEncodingType;

typedef union {
    Word word;
    struct {
        unsigned int encodingType: 2;
        unsigned int destinationAddressType: 3;
        unsigned int opcode: 4;
        unsigned int sourceAddressType: 3;
    } fields;
} InstructionWord;

typedef union {
    Word word;
    struct {
        unsigned int encodingType: 2;
        unsigned int immediateValue: 10;
    } fields;
} ImmediateOperandWord;

typedef union {
    Word word;
    struct {
        unsigned int encodingType: 2;
        unsigned int address: 10;
    } fields;
} DirectOperandWord;

typedef union {
    Word word;
    struct {
        unsigned int encodingType: 2;
        unsigned int sourceRegister: 5;
        unsigned int destinationRegister: 5;
    } fields;
} RegisterOperandWord;

typedef struct {
    const char * operation;
    const unsigned int code;
    const unsigned int addressTypes[2];
} InstructionModel;

const InstructionModel * findInstructionModel(const char * operation);
unsigned int getModelOperandsCount(const InstructionModel * model);
OperandAddressType oeprandStringToAddressType(const char * argument);
int getDataWordsCount(OperandAddressType sourceAddressType, OperandAddressType destinationAddressType);
int registerIndexFromArgumentString(const char * argumentString);

#endif /* __INSTRUCTIONS_H__ */