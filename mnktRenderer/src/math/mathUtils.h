
/**
 * @file mathUtils.h
 *
 * Contains definition of common math utility functions
*/

#ifndef MNKT_MATH_UTILS_H
#define MNKT_MATH_UTILS_H


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
float   mnkt_clamp(float value, float min, float max);


#endif // MNKT_MATH_UTILS_H
