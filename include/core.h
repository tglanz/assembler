#ifndef __CORE_H__
#define __CORE_H__

typedef enum {
    true = 1,
    false = 0
} bool;

enum {
    EXIT_CODE_SUCCESS = 0,
    EXIT_CODE_UNKNOWN = -1,
    EXIT_CODE_FATAL = -2
};


/* Encoding type (A,R,E bits) */
enum {
    ENCODING_TYPE_ABSOLUTE    = 0x0,
    ENCODING_TYPE_EXTERNAL    = 0x1,
    ENCODING_TYPE_RELOCATBALE = 0x3
};

enum {
    ADDRESS_TYPE_IMMEDIATE   = 0x1,
    ADDRESS_TYPE_DIRECT      = 0x3,
    ADDRESS_TYPE_REGISTER    = 0x5
};

#endif /* __CORE_H__ */