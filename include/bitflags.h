#ifndef __BITFLAGS_H__
#define __BITFLAGS_H__

#include "core.h"

bool has_flag(uint flag, uint flags);
void set_flag(uint flag, uint * flags);

#endif /* __BITFLAGS_H__ */