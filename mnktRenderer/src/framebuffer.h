
/*
 * @file framebuffer.h
 *
 * Defines the Framebuffer struct and its API
*/

#ifndef MNKT_FRAMEBUFFER_H
#define MNKT_FRAMEBUFFER_H

#include <stdint.h>
#include <string.h>


/**
 * @struct Framebuffer
 * Target memory areas on which all rendering operation are performed.
 * Contains the RGB bitmap image that can be presented on screen after rendering.
*/
typedef struct {
        uint32_t        width;                  ///< Width of the framebuffer image expressed in pixels
        uint32_t        height;                 ///< Height of the framebuffer image expressed in pixels
        unsigned char*  colorBuffer;            ///< Stores pixels colors in RGB format, must point to an array of 3 * width * height elements
        float*          depthBuffer;            ///< Stores a depth value for each pixel of the color buffer, must point to an array of width * height elements
} Framebuffer_t;


/**
 * @function mnkt_framebuffer_clearColor
 * Sets the color of all pixels inside the framebuffer
 * @param r Red value to be used for the clear color
 * @param g Green value to be used for the clear color
 * @param b Blue value to be used for the clear color
 * @param framebuffer Framebuffer of which the color buffer must be cleared
*/
void mnkt_framebuffer_clearColor(const unsigned char r, const unsigned char g, const unsigned char b, Framebuffer_t* framebuffer);


/**
 * @function mnkt_framebuffer_clearDepth
 * Sets the depth values of all pixels inside the framebuffer
 * @param depth Depth value to be used for all pixels
 * @param framebuffer Framebuffer of which the depth buffer must be cleared
*/
void mnkt_framebuffer_clearDepth(float depth, Framebuffer_t* framebuffer);



#endif // MNKT_FRAMEBUFFER_H

