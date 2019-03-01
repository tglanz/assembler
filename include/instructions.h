/**
 * Instructions related definitions
 */
#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

/* standard */
#include <string.h>

/* services */
#include "core.h"
#include "log.h"
#include "parsing.h"
#include "words.h"
#include "bitflags.h"

#define REGISTER_ADDRESS_PREFIX ("@r")
#define REGISTER_MIN (0)
#define REGISTER_MAX (7)
#define INVALID_REGISTER (REGISTER_MAX + 1)

/**
 * Address types
 */
typedef enum {
    /* unknown, default or invalid */
    ADDRESS_TYPE_NONE        = 0x0,
    /* constant */
    ADDRESS_TYPE_IMMEDIATE   = 0x1,
    /* address */
    ADDRESS_TYPE_DIRECT      = 0x3,
    /* register */
    ADDRESS_TYPE_REGISTER    = 0x5
} OperandAddressType;

/**
 * Encoding type (A,R,E bits)
 */
typedef enum {
    /* indicate that the evaluation is invariant to position */
    ENCODING_TYPE_ABSOLUTE    = 0x0,
    /* indicate that the evaluation dependant on an external address */
    ENCODING_TYPE_EXTERNAL    = 0x1,
    /* indicate that the evaluation is depndant on position */
    ENCODING_TYPE_RELOCATBALE = 0x2
} InstructionEncodingType;

/**
 * Word interpertation as an instruction
 */
typedef union {
    Word word;
    struct {
        /* the encoding type (EncodingType) */
        uint encodingType: 2;
        /* address type (OperandAddressType) of the destination operand */
        uint destinationAddressType: 3;
        /* the opcode - 4 bits, indicating the operation */
        uint opcode: 4;
        /* address type (OperandAddressType) of the source operand */
        uint sourceAddressType: 3;
    } fields;
} InstructionWord;

/**
 * Word interpertation as an immediate operand data word
 */
typedef union {
    Word word;
    struct {
        /* the encoding type (EncodingType) */
        uint encodingType: 2;
        /* 10bits, 2s complement representation of a signed value */
        uint immediateValue: 10;
    } fields;
} ImmediateOperandWord;

/**
 * Word interpertation as a direct operand data word
 */
typedef union {
    Word word;
    struct {
        /* the encoding type (EncodingType) */
        uint encodingType: 2;
        /* the address value */
        uint address: 10;
    } fields;
} DirectOperandWord;

/**
 * Word interpertation as a direct operand data word
 */
typedef union {
    Word word;
    struct {
        /* the encoding type (EncodingType) */
        uint encodingType: 2;
        /* destination register */
        uint destinationRegister: 5;
        /* source register */
        uint sourceRegister: 5;
    } fields;
} RegisterOperandWord;

/**
 * An instruction specification - used to encode and validate according to a centralized definition
 */
typedef struct {
    /* operation name */
    string operation;

    /* the operation code */
    const uint code;

    /**
     * Available address types for source and destination operands.
     * The address types are bitflags, representing all of the available options,
     * such as (ADDRESS_TYPE_IMMEDIATE | ADDRESS_TYPE_REGISTER)
     * source available address types are at addressTypes[0]
     * destination available address types are at addressTypes[1]
     */
    const uint addressTypes[2];
} InstructionModel;

/**
 * Given an operation name, find an instruction model with the same operation name
 * 
 * Arguments
 *      operation: string - the operation name to look for
 * 
 * Return
 *      const InstructionModel* - an immutable reference to an instruction model
 *              NULL if not found
 */
const InstructionModel * findInstructionModel(string operation);

/**
 * Calculate, according to a given model, how many operand the relvant instruction should have
 * 
 * Arguments
 *      model: const InstructionModel* - an immutable reference to an instruction model
 * 
 * Returns
 *      uint: the number of operands
 */
uint getModelOperandsCount(const InstructionModel * model);

/**
 * Given a string, figure out what is the address type of an operand
 * 
 * Arguments
 *      argument: string - string of an operand to get the address type of
 * 
 * Returns
 *      OperandAddressType: the assumed operand address type
 */
OperandAddressType oeprandStringToAddressType(string argument);

/**
 * Calculate how many additional data words (for operands) and instruction should have.
 * This is dependant solely on the source/destination address types.
 * 
 * Arguments
 *      sourceAddressType: OperandAddressType - address type of the source operand
 *      destinationAddressType: OperandAddressType - address type of the destination operand
 */
uint getDataWordsCount(OperandAddressType sourceAddressType, OperandAddressType destinationAddressType);

/**
 * Given an operartion argument string,
 * assumed to be of a register operand, get the register index it represents
 * 
 * Example
 *      "@r3" -> 3
 *      "asd" -> INVALID_REGISTER
 * 
 * Arguments
 *      argumentString: string - the operand string
 * 
 * Returns
 *      uint - the register index. If invalid, returns INVALID_REGISTER
 */
uint registerIndexFromArgumentString(string argumentString);

#endif /* __INSTRUCTIONS_H__ */