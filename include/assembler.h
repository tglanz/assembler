#ifndef __POCESS_H__
#define __POCESS_H__

#include "core.h"
#include "log.h"
#include "parser.h"

#define MAX_FILE_PATH_LENGTH (256)
#define MAX_LINE_LENGTH (80)

typedef struct {
    unsigned int IC;
    unsigned int DC;

    bool firstPassSuccess;
    bool secondPassSuccess;
} State;

void assemblerResetState(State * state);
void assemblerLogState(const State * state);

void assemblerProcessInput(const char * baseName);

void assemblerRunFirstPass(State * state, FILE * file);
void assemblerRunSecondPass(State * state, FILE * file);

void assemblerGenerateOutputs(State * state, const char * baseName);

void assemblerFileName(char * destination, const char * baseName, const char * extension);

#endif /* __POCESS_H__ */