#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <ctype.h>
#include <string.h>

#include "core.h"
#include "log.h"
#include "sourceFile.h"
#include "assembler/assemblyState.h"
#include "assembler/firstPass.h"
#include "parsing.h"
#include "symbols.h"
#include "words.h"
#include "instructions.h"


bool assembleInput(const char * baseName);

#endif /* __ASSEMBLER_H__ */
