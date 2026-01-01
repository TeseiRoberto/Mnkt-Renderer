
/**
 * @file mathUtils.c
 *
 * Contains implementation of common math utility functions
*/

#include "mathUtils.h"


/**
 * @function mnkt_clamp
 * Clamps the given value to the range [min, max]
 * @param value The value to be clamped
 * @param min Minimum value that the returned value can have
 * @param max Maximum value that the returned value can have
 * @return value, if it is in range [min, max].
 * min, if value is smaller than min.
 * max, if value is greater than max.
*/
float mnkt_clamp(float value, float min, float max)
{
        if(value < min)
                return min;

        return value > max ? max : value;
}

