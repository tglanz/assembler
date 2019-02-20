#include "parsing.h"

bool tryGetLabel(const char * line, char * buffer) {
    return sscanf(line, "%s: ", buffer) == 1;
}

bool tryGetDirective(const char * line, char * buffer){
    return sscanf(line, ".%s ", buffer) == 1;
}