
/**
 * @file image.h
 *
 * Defines the Image_t struct.
*/

#ifndef MNKT_IMAGE_H
#define MNKT_IMAGE_H

#include <stdint.h>
#include <stddef.h>


/**
 * @struct Image
 * Models a 2D, RGBA encoded, image.
*/
typedef struct {
        uint32_t        width;                  ///< Width of the image
        uint32_t        height;                 ///< Height of the image
        int*            pixels;                 ///< The pixels of the image packed as 32 bit integers in RGBA encoding
} Image_t;


#endif // MNKT_IMAGE_H

