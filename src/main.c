#include <stdio.h>

#include "core.h"
#include "log.h"
#include "assembler.h"

int main(int argc, char ** argv){
    int idx, tmp;

    for (idx = 1; idx < argc; ++idx){
        if (sscanf(argv[idx], "-v%d", &tmp) == 1){
            logSetLevel(tmp);
        } else {
            assembleInput(argv[idx]);
        }
    }

    return EXIT_CODE_UNKNOWN;
}