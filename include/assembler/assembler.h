/**
 * The actual assembler logic is defined in here.
 * The implementation of the assembler relies on joining the utilities provided in this project.
 * 
 * By design, the assembler module should be the only part making and modifications to it's state.
 * Other parts of the program acts as services for it.
 * 
 * As an overview, assuming the assembler received an input of `input` to compile
 * 
 * - perform a first iteration on the file `input.as` (aka firstPass) which should do the following
 *      1. fill in the symbols table
 *      2. fill in the in memory data (.data and .string define the data values)
 *      3. encode the main instruction for each encountered operation
 *      4. add additional data words, as placeholders, in the instructions for each operand of the operation
 * 
 * - perform a second iteration on the file `input.as` (aka secondPass) which should do the following
 *      1. handle .entry directives, meaningly marking the relevant labels as entries. This could not be done
 *         earlier, because we might have not yet collected all the labels, and order is not assumed
 *      2. fill the encoding for each operand word, which was left as a placeholder in the first pass
 *      3. for each externalized label, remember the location in the instructions space it should point to
 * 
 * Generally this is an implementation detail and can be changed. The assembler provides a single entry point,
 * so that the implementation details reside internally.
 */
#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

/* standard */
#include <ctype.h>
#include <string.h>

/* services */
#include "core.h"
#include "log.h"
#include "sourceFile.h"
#include "instructions.h"

/* assembler parts */
#include "assembler/assemblyState.h"
#include "assembler/firstPass.h"
#include "assembler/secondPass.h"
#include "assembler/outputs.h"

/**
 * The offset, in memory,
 * from which the instructions should be loaded from
 */
#define INSTRUCTIONS_OFFSET (100)

/**
 * The single entry point to the assembler.
 * Assembles a given input and notifies on success or failure.
 * 
 * Arguments
 *      baseName: string - the input name.
 *          the exact file names will be constructed using this value.
 *          for example, for baseName = 'path/something',
 *          the file 'path/something.as' will be compiled
 * 
 * Returns
 *      bool - true only if the process had ran to completion, successfuly
 */
bool assembleInput(string baseName);

#endif /* __ASSEMBLER_H__ */
