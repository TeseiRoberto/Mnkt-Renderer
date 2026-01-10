
/**
 * @file colorUtils.c
 *
 * Contains implementation of utility functions to work with colors
*/


#include "../math/mathUtils.h"


/**
 * @function mnkt_colorAsUChar
 * Converts the given color from float to the corresponding unsigned char value.
 * @param color The value to be converted.
 *      Should be in range [0.0f, 1.0f], a greater value will be clamped
 * @return A value in the range [0, 255] that represents the given color value
*/
unsigned char mnkt_colorAsUChar(float color)
{
        color = mnkt_math_clamp(color, 0.0f, 1.0f);

        return color * 255;
}


/**
 * @function mnkt_colorAsFloat
 * Converts the given color from an unsigned char to a normalized float value.
 * @param color The value to be converted.
 * @return A float value in the range [0.0f, 1.0f] that represents the given color value
*/
float mnkt_colorAsFloat(unsigned char color)
{
        return (float) color / 255.0f;
}
