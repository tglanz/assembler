#include "parser.h"

StatementType parserDetermineStatementType(const char * line) {
    if (strlen(line) == 0 || line[0] == COMMENT_CHAR){
        return STATEMENT_TYPE_COMMENT;
    }

    return STATEMENT_TYPE_UNKNOWN;
}