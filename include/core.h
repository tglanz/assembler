/**
 * Core types used accross the application
 */
#ifndef __CORE_H__
#define __CORE_H__

/* immutable array of characters */
typedef const char * string;

/* unsigned integer */
typedef unsigned int uint;

/* to avoid multiple kinds of truthy values */
typedef enum {
    true = 1,
    false = 0
} bool;

/**
 * perhaps we could add ones to indicate the kind of failure
 * that ocurred during the assembly process?
 */
enum {
    EXIT_CODE_SUCCESS = 0,
    EXIT_CODE_UNKNOWN = -1
};

#endif /* __CORE_H__ */