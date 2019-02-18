#include <stdio.h>

#include "core.h"
#include "log.h"
#include "assembler.h"

int main(int argc, char ** argv){
    int idx;

    for (idx = 1; idx < argc; ++idx){
        assemblerProcessInput(argv[idx]);
    }

    return EXIT_CODE_UNKNOWN;
}