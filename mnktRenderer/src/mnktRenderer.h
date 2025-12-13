
/**
 * @file mnktRenderer.h
 *
 * Defines the graphics API implemented by the mnktRenderer.
*/

#ifndef MNKT_RENDERER
#define MNKT_RENDERER

#include <stdint.h>
#include <stddef.h>

#include "framebuffer.h"


/**
 * @struct Image
 * Models a 2D, RGBA encoded, image.
*/
typedef struct {
        uint32_t        width;                  ///< Width of the image
        uint32_t        height;                 ///< Height of the image
        int*            pixels;                 ///< The pixels of the image packed as 32 bit integers in RGBA encoding
} Image_t;



/**
 * @function mnkt_drawPoint
 * Draws a sequence of 2D points
 * @param vertices vertices that defines the points to be drawn
 * @param verticesCount Number of elements stored in the given vertices array
 * @param fb Framebuffer on which the rendered points should be outputted
*/
void mnkt_drawPoint(float* vertices, const size_t verticesCount, /*TODO: Add shaders */ Framebuffer_t* fb);


/**
 * @function mnkt_drawLine
 * Draws a 2D segmented line
 * @param vertices vertices that defines the line to be drawn
 * @param verticesCount Number of elements stored in the given vertices array
 * @param fb Framebuffer on which the rendered line should be outputted
*/
void mnkt_drawLine(float* vertices, const size_t verticesCount, /*TODO: Add shaders */ Framebuffer_t* fb);


/**
 * @function mnkt_draw
 * Draws a sequence of triangles
 * @param vertices vertices that defines the triangles to be drawn
 * @param verticesCount Number of elements stored in the given vertices array
 * @param fb Framebuffer on which the rendered triangles should be outputted
*/
void mnkt_draw(float* vertices, const size_t verticesCount, /*TODO: Add shaders */ Framebuffer_t* fb);


#endif // MNKT_RENDERER


