#include "bitflags.h"

bool has_flag(uint flag, uint flags) {
    return (flag & flags) == flag;
}

void set_flag(uint flag, uint * flags) {
    (*flags) |= flag;
}