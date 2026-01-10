
/**
 * @file mathUtils.c
 *
 * Contains implementation of common math and geometrical utility functions
*/

#include "mathUtils.h"


/**
 * @function mnkt_math_clamp
 * Clamps the given value to the range [min, max]
 * @param value The value to be clamped
 * @param min Minimum value that the returned value can have
 * @param max Maximum value that the returned value can have
 * @return value, if it is in range [min, max].
 * min, if value is smaller than min.
 * max, if value is greater than max.
*/
float mnkt_math_clamp(float value, float min, float max)
{
        if(value < min)
                return min;

        return value > max ? max : value;
}


/**
 * @function mnkt_math_pointIntersectRect
 * Checks if the given point is inside, or on the border of, the given rectangle.
 * @param x X coordinate of the point to be checked
 * @param y Y coordinate of the point to be checked
 * @param minRectX X coordinate of the rectangle's top left corner
 * @param minRectY Y coordinate of the rectangle's top left corner
 * @param maxRectX X coordinate of the rectangle's bottom right corner
 * @param maxRectY Y coordinate of the rectangle's bottom right corner
 * @return One if the point is inside the specified rectangle or on its border, zero if it is outside
*/
int mnkt_math_pointIntersectRect(float x, float y, float minRectX, float minRectY, float maxRectX, float maxRectY)
{
        if(x < minRectX || x > maxRectX)
                return 0;

        if(y < minRectY || y > maxRectY)
                return 0;

        return 1;
}


