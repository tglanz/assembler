/**
 * Defines the part of the assembler that should generate the output files.
 * 
 * This is the single module that should be aware of the required formats, and should provide
 * a single procedure to create each one of the file types.
 * 
 * Should not mutate any state
 */
#ifndef __OUTPUTS_H__
#define __OUTPUTS_H__

#include "core.h"
#include "log.h"
#include "base64.h"

#include "assembler/assembler.h"

/**
 * Generate an entries file according to a given state of the assembler
 * 
 * 
 * Arguments
 *      path: string - the path to write the file to. overwrite if already exists
 *      state: const AssemblyState* - an immutable pointer to the assembler state
 * 
 * File format
 *      label:string     address:number
 *      label:string     address:number
 *      .
 *      .
 *      .
 *      label:string     address:number
 */
void generateEntriesFile(string path, const AssemblyState * state);

/**
 * Generate an entries file according to a given state of the assembler
 * 
 * Arguments
 *      path: string - the path to write the file to. overwrite if already exists
 *      state: const AssemblyState* - an immutable pointer to the assembler state
 * 
 * File format
 *      label:string     address:number
 *      label:string     address:number
 *      .
 *      .
 *      .
 *      label:string     address:number
 */
void generateExternalsFile(string path, const AssemblyState * state);

/**
 * Generate an entries file according to a given state of the assembler
 * 
 * Arguments
 *      path: string - the path to write the file to. overwrite if already exists
 *      state: const AssemblyState* - an immutable pointer to the assembler state
 * 
  * File format
 *      (instructions count :number)     (data count :number)
 *      (base64 encoded character)(base64 encoded character)
 *      (base64 encoded character)(base64 encoded character)
 *      .
 *      .
 *      .
 *      (base64 encoded character)(base64 encoded character)
 */
void generateObjectFile(string path, const AssemblyState * state);

#endif /* __OUTPUTS_H__ */