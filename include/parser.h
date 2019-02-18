#ifndef __PARSER_H__
#define __PARSER_H__

#include <ctype.h>
#include <string.h>

#include "core.h"
#include "log.h"

#define COMMENT_CHAR (';')

typedef enum {
    STATEMENT_TYPE_EMPTY,
    STATEMENT_TYPE_COMMENT,
    STATEMENT_TYPE_DIRECTIVE,
    STATEMENT_TYPE_INSTRUCTION,
    STATEMENT_TYPE_UNKNOWN
} StatementType;

StatementType parserDetermineStatementType(const char * line);

#endif /* __PARSER_H__ */