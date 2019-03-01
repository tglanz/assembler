/**
 * Define interface of the second pass.
 * Only a single entry point is required, to execute the pass.
 * 
 * For additional information on what the pass should do,
 * see the documentation at ./assembler.h
 */
#ifndef __SECOND_PASS_H__
#define __SECOND_PASS_H__

#include "assembler/assembler.h"

/**
 * Execute the second pass
 * 
 * Arguments
 *      state: AssemblyState* - the state of the asssembler to work upon
 *      sourceFile: SourceFile* - the source file to read the program from
 */
void runSecondPass(AssemblyState * state, SourceFile * sourceFile);

#endif /* __SECOND_PASS_H__ */