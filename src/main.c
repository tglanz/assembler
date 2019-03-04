#include <stdio.h>

#include "core.h"
#include "log.h"

#include "assembler/assembler.h"

int main(int argc, char ** argv){
    /* declerations */
    bool someFailed = false; /* keep track on whether he encountered failures */
    int idx, tmp;

    /* iterate through arguments and delegate required actions */
    for (idx = 1; idx < argc; ++idx){
        if (sscanf(argv[idx], "-v%d", &tmp) == 1){
            logSetLevel(tmp);
        } else {
            logInfo("Assembling: %s", argv[idx]);
            if (!assembleInput(argv[idx])){
                logError("Failed to compile: %s", argv[idx]);
                someFailed = true;
            }
        }
    }

    /* provide a summary, return valid exit codes */
    if (someFailed){
        logWarning("Done, one or more inputs had failed to compile");
        return EXIT_CODE_UNKNOWN;
    } else {
        logInfo("Done, all inputs have been compiled successfully");
        return EXIT_CODE_SUCCESS;
    }
}