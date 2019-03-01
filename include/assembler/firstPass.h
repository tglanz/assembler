/**
 * Define interface of the first pass.
 * Only a single entry point is required, to execute the pass.
 * 
 * For additional information on what the pass should do,
 * see the documentation at ./assembler.h
 */
#ifndef __FIRST_PASS_H__
#define __FIRST_PASS_H__

#include "assembler/assembler.h"

/**
 * Execute the first pass
 * 
 * Arguments
 *      state: AssemblyState* - the state of the asssembler to work upon
 *      sourceFile: SourceFile* - the source file to read the program from
 */
void runFirstPass(AssemblyState * state, SourceFile * sourceFile);

#endif /* __FIRST_PASS_H__ */