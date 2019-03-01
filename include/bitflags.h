/**
 * Definitions to help with bitflags calculations
 */
#ifndef __BITFLAGS_H__
#define __BITFLAGS_H__

#include "core.h"

/**
 * Check whether a flag is on
 * 
 * Arguments
 *      flag: uint - the flag to check if is set or unset
 *      flags: uint - the flags to check whether flag is set or unset in
 * 
 * Returns
 *      bool - true only if the flag is set
 */
bool has_flag(uint flag, uint flags);

/**
 * Set a flag
 * 
 * Arguments
 *      flag: uint - the flag to set
 *      flags: uint* - the flags to set the flag in
 */
void set_flag(uint flag, uint * flags);

/**
 * Other implemntation can be made, such as unset_flag, reset_flags etc.. but there was no need for those
 */

#endif /* __BITFLAGS_H__ */