#ifndef __CORE_H__
#define __CORE_H__

/* immutable array of characters */
typedef const char * string;

/* uint, just too common */
typedef unsigned int uint;

/* avoid multiple kinds of truthy values */
typedef enum {
    true = 1,
    false = 0
} bool;

enum {
    EXIT_CODE_SUCCESS = 0,
    EXIT_CODE_UNKNOWN = -1
};

#endif /* __CORE_H__ */