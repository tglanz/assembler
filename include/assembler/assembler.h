#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <ctype.h>
#include <string.h>

#include "core.h"
#include "log.h"
#include "sourceFile.h"
#include "instructions.h"

#include "assembler/assemblyState.h"
#include "assembler/firstPass.h"
#include "assembler/secondPass.h"
#include "assembler/outputs.h"

#define INSTRUCTIONS_OFFSET (100)

bool assembleInput(string baseName);

#endif /* __ASSEMBLER_H__ */
