#ifndef __OUTPUTS_H__
#define __OUTPUTS_H__

#include "core.h"
#include "log.h"
#include "base64.h"

#include "assembler/assembler.h"

void generateEntriesFile(string path, const AssemblyState * state);
void generateExternalsFile(string path, const AssemblyState * state);
void generateObjectFile(string path, const AssemblyState * state);

#endif /* __OUTPUTS_H__ */