#ifndef __PARSING_H__
#define __PARSING_H__

#include <stdio.h>

#include "core.h"

bool tryGetLabel(const char * line, char * buffer);
bool tryGetDirective(const char * line, char * buffer);

#endif /* __PARSING_H__ */